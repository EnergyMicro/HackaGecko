/**************************************************************************//**
 * @file charlie_led.h
 * @brief charlie_led.h
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
#ifndef CHARLIE_LED
#define CHARLIE_LED

#include <stdint.h>

/* Defines the maxiumum number of leds in one charlieplex struct. */
/* You can have less than this by adjusting num_leds in the struct */
#define MAX_LEDS 12
/* Defines how many intensity levels there are */
#define MAX_FRAMECOUNT 10

typedef struct 
{
  uint8_t numLeds;
  uint8_t ledPortsP[MAX_LEDS];
  uint8_t ledPinsP[MAX_LEDS];
  uint8_t ledPortsN[MAX_LEDS];
  uint8_t ledPinsN[MAX_LEDS];
  uint8_t ledValue[MAX_LEDS];
  uint8_t frameCnt;
  uint8_t ledCnt;
} charlieLedConfig_t;

void CLED_Init(void);
void CLED_deInit(void);

void CLED_Advance(void);

void CLED_SetLed(uint8_t config, uint8_t led, uint8_t value);

#endif
