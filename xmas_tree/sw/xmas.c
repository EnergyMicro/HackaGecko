/**************************************************************************//**
 * @file xmas.c
 * @brief xmas.c
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
#include "xmas_lights.h"
#include "charlie_led.h"
#include "lesense_letouch.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_rtc.h"
#include "random_gen.h"

#include <stdio.h>

#define MAX_ON_COUNTER 200

#define LIGHTUP_SPEED 500
#define INTENSITY_LOW     200000
#define INTENSITY_HIGH    10000

volatile uint32_t rtcTicks = 0;

volatile uint32_t eventTicks[12];

bool highIntensity = false;

int IntensityGet(void)
{
  if (highIntensity) {
    return INTENSITY_HIGH;
  }
  return INTENSITY_LOW;
}


void HandleLed(int led)
{
  uint32_t tick = eventTicks[led];
  uint8_t red = LIGHTS_GetRed(led);
  uint8_t green = LIGHTS_GetGreen(led);
  

  if (tick % 5 == 0)
  {
    green += 1;
    red += 1;
  }
  else
  {
    if (tick & 1)
    {
      green += 1;
    }
    else
    {
      red += 1;
    }
  }
  LIGHTS_Set(led, red, green);
  if (red > 10 || green > 10)
  {
    LIGHTS_Set(led, 0, 0);
    eventTicks[led] += (genrand() % IntensityGet()) + 10;
  }
  else
  {
    eventTicks[led] += LIGHTUP_SPEED;
  }
}

void HandleEvents()
{
  int i;
  uint32_t curTick = rtcTicks;
  for(i=0; i < 12; i++)
  {
    if (eventTicks[i] < curTick)
    {
      HandleLed(i);
    }
  }
}

void Delay(uint32_t ticks)
{
  uint32_t nowTicks = rtcTicks;
  
  while (rtcTicks - nowTicks < ticks) ;
}

void RTC_IRQHandler()
{
  /* Clear interrupt source */
  RTC_IntClear(RTC_IF_COMP0);
  
  /* TODO: Handle overflows! */
  rtcTicks++;

  /* Handle events */
  HandleEvents();
  
  /* Update lights */
  CLED_Advance();
  
  /* Lesense recalibration every 5 seconds */
  if ((rtcTicks % (5 * 32768)) == 0)
  {
    LETOUCH_Calibration();
  }
  

  
}

void setupRTC(void)
{
   /* Set up RTC for interrupts */
  /* Use LFRCO for timing */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
  CMU_ClockEnable(cmuClock_CORELE, true);
  CMU_ClockEnable(cmuClock_RTC, true);
  
  /* Default setup of RTC. */
  RTC_Init_TypeDef init = RTC_INIT_DEFAULT;
  init.enable = false;
  RTC_Init(&init);
  
  /* Trigger every tick. */
  RTC_CompareSet(0, 1);

  /* Enable interrupt on COMP0 */
  RTC_IntEnable(RTC_IF_COMP0);
  NVIC_ClearPendingIRQ(RTC_IRQn);
  NVIC_EnableIRQ(RTC_IRQn);

  /* Start RTC */
  RTC_Enable(true);
}

void RTC_DeInit(void)
{
  RTC_Enable(false);
  /* Clear interrupt source */
  RTC_IntClear(RTC_IF_COMP0);
  NVIC_ClearPendingIRQ(RTC_IRQn);
}
  
  

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  int i, timeOut = 0;
  
  float sensitivity[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 15.0};
  
  /* Chip errata */
  CHIP_Init();

  /* Initialize Random number generator */
  genrand_init(0x0, 0x20, 0x30, 0x40);
  
  /* Init lights */
  LIGHTS_Init();
  
  /* RTC is used for Ligts */
  setupRTC();
    
  /* Initialize TOUCH on lesense */
  LETOUCH_Init(sensitivity);
  
  /* Cozy orange light :p */
  for (i=0; i < 12; i++)
  {
    LIGHTS_Set(i, 0, 0);
    /* Initial schedule */
    eventTicks[i] = (genrand() % IntensityGet()) + 10;
  }
  
  
  while(1) {
   EMU_EnterEM2(false);
   if (LETOUCH_GetChannelsTouched() != 0)
   {
     if (timeOut == 0)
     {
      timeOut = 1000000;
      highIntensity = true;
      for (i=0; i < 12; i++)
      {
       eventTicks[i] = rtcTicks+10; 
      }
     }
   }
   if (timeOut > 0)
   {
     timeOut--;
   }
   else
   {
     highIntensity = false;
   }
  }

}