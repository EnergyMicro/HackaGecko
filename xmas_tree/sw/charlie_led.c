/**************************************************************************//**
 * @file charlie_led.c
 * @brief charlie_led.c
 * @author Energy Micro AS
 * @version 1.0.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#include "charlie_led.h"

#include "em_device.h"
#include "em_gpio.h"
#include "em_rtc.h"
#include "em_cmu.h"

/* This is the mapping of the LEDs. These are the pins that
   connect the different LEDs */

#define NUM_CONFIGS 2

static charlieLedConfig_t ledsFront1 = {
  .numLeds     = 12,
  .ledPortsP   = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  .ledPinsP    = { 0, 3, 0, 1, 1, 3, 1, 2, 0, 2, 2, 3},
  .ledPortsN   = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  .ledPinsN    = { 3, 0, 1, 0, 3, 1, 2, 1, 2, 0, 3, 2},
  .ledValue    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  .frameCnt    = 0,
  .ledCnt      = 0,
};

static charlieLedConfig_t ledsBack1 = {
  .numLeds     = 12,
  .ledPortsP   = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  .ledPinsP    = { 8, 9, 8, 10, 8, 11, 9, 10, 9, 11, 10, 11},
  .ledPortsN   = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  .ledPinsN    = { 9, 8, 10, 8, 11, 8, 10, 9, 11, 9, 11, 10},
  .ledValue    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  .frameCnt    = 0,
  .ledCnt      = 0,
};

#if 0
static charlieLedConfig_t ledsBack2 = {
  .numLeds     = 12,
  .ledPortsP   = { 4,  4,  4,  4,  4,  2,  4,  4,  4,  2,  4,   2},
  .ledPinsP    = { 10, 12, 10, 13, 10, 14, 12, 13, 12, 14, 13, 14},
  .ledPortsN   = { 4,  4,  4,  4,  2,  4,  4,  4,   2, 4,   2,  4},
  .ledPinsN    = { 12, 10, 13, 10, 14, 10, 13, 12, 14, 12, 14, 13},
  .ledValue    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  .frameCnt    = 0,
  .ledCnt      = 0,
};
#endif


static charlieLedConfig_t *configs[] = {&ledsFront1, &ledsBack1};

/* One mask per port */
static uint32_t doutMask[6];
static uint32_t modelMask[6];
static uint32_t modehMask[6];

void CLED_Advance()
{
  int i,j;

  /* Iterate through all ports and reset GPIO */
  for (i=0; i < 6; i++)
  {
    GPIO->P[i].DOUT  &= doutMask[i];
    GPIO->P[i].MODEL &= modelMask[i];
    GPIO->P[i].MODEH &= modehMask[i];
  }
  /* Iterate through all configs */
  for (j = 0; j < NUM_CONFIGS; j++)
  {
    charlieLedConfig_t *c = configs[j];

    c->ledCnt++;
    if (c->ledCnt > c->numLeds )
    {
      c->ledCnt = 0;
      c->frameCnt++;
      if (c->frameCnt > MAX_FRAMECOUNT)
      {
        c->frameCnt = 0;
      }
    }
    /* Check if this LED should be lighted. If not then exit */
    if (c->ledValue[c->ledCnt] > c->frameCnt)
    {
      uint8_t led   = c->ledCnt;
      uint8_t pinP  = c->ledPinsP[led];
      uint8_t portP = c->ledPortsP[led];
      uint8_t pinN  = c->ledPinsN[led];
      uint8_t portN = c->ledPortsN[led];

      /* Ligt up this LED by setting p high, while n is low. */
      GPIO->P[portP].DOUT  |= 1 << pinP;
      if (pinP < 8)
      {
        GPIO->P[portP].MODEL |= _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE << (pinP * 4);
      }
      else
      {
        GPIO->P[portP].MODEH |= _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE << ((pinP-8) * 4);
      }
      if (pinN < 8)
      {
        GPIO->P[portN].MODEL |= _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE << (pinN * 4);
      }
      else
      {
        GPIO->P[portN].MODEH |= _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE << ((pinN-8) * 4);
      }
    }
  }
}

void CLED_Init(void)
{
  int i,j;

  /* Clear masks */
  for (i=0; i< 6; i++)
  {
    doutMask[i]  = 0xFFFFFFFF;
    modelMask[i] = 0xFFFFFFFF;
    modehMask[i] = 0xFFFFFFFF;
  }

  /* Iterate through all configs */
  for (j = 0; j < NUM_CONFIGS; j++)
  {
    charlieLedConfig_t *c = configs[j];

    /* Build masks */
    for (i=0; i< c->numLeds; i++)
    {
      doutMask[c->ledPortsP[i]] &=  ~(1 << c->ledPinsP[i]);
      doutMask[c->ledPortsN[i]] &=  ~(1 << c->ledPinsN[i]);

      if (c->ledPinsP[i] < 8)
      {
        modelMask[c->ledPortsP[i]] &= ~(0xF << (c->ledPinsP[i] * 4));
      }
      else
      {
        modehMask[c->ledPortsP[i]] &= ~(0xF << ((c->ledPinsP[i] - 8) * 4));
      }
      if (c->ledPinsN[i] < 8)
      {
        modelMask[c->ledPortsN[i]] &= ~(0xF << (c->ledPinsN[i] * 4));
      }
      else
      {
        modehMask[c->ledPortsN[i]] &= ~(0xF << ((c->ledPinsN[i] - 8) * 4));
      }
    }
  }

  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Set drivestrength to 20mA for brighter LEDs */
  for (i=0; i < 6; i++)
  {
    GPIO->P[i].CTRL = GPIO_P_CTRL_DRIVEMODE_HIGH;
  }
}

void CLED_deInit(void){

  int i;

  /* Iterate through all ports and reset GPIO */
  for (i=0; i < 6; i++)
  {
    GPIO->P[i].DOUT  = 0;
    GPIO->P[i].MODEL = 0;
    GPIO->P[i].MODEH = 0;
  }
}

void CLED_SetLed(uint8_t config, uint8_t led, uint8_t value)
{
  if (config < NUM_CONFIGS)
  {
    charlieLedConfig_t *c = configs[config];

    if (led < c->numLeds)
    {
      c->ledValue[led] = value;
    }
  }
}
