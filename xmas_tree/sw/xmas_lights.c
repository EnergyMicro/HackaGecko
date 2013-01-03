/**************************************************************************//**
 * @file xmas_lights.c
 * @brief xmas_lights.c
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

uint8_t greenValue[12];
uint8_t redValue[12];


void LIGHTS_Init(void)
{
  CLED_Init();
}

void LIGHTS_deInit(void)
{
  CLED_deInit();
}

uint8_t LIGHTS_GetGreen(uint8_t led)
{
  return greenValue[led];
}

uint8_t LIGHTS_GetRed(uint8_t led)
{
  return redValue[led];
}

void LIGHTS_Set(uint8_t led, uint8_t red, uint8_t green)
{
  greenValue[led] = green;
  redValue[led] = red;
  
  uint8_t config = 0;
  if (led < 6)
  {
    config = 0;
  }
  else
  {
    config = 1;
    led -= 6;
  }
  
  /* Set red first */
  CLED_SetLed(config, led * 2, red);
  /* Then green */
  CLED_SetLed(config, led * 2 + 1, green);  
}