/**************************************************************************//**
 * @file
 * @brief Ikea DIODER PWM driver
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
#ifndef __IKEADIODER_H_
#define __IKEADIODER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#ifndef IKEA_TIMER
#define IKEA_TIMER TIMER0
#endif
#ifndef IKEA_TIMER_CMU
#define IKEA_TIMER_CMU cmuClock_TIMER0
#endif
#ifndef IKEA_TIMER_TOP 
#define IKEA_TIMER_TOP 0xFE /* max value for pwm is TOP+1*/ 
#endif
  
#define GG_STK_SETTINGS true  
  
void IKEADIODER_Init(void);
void IKEADIODER_RGBColorSet(uint8_t red, uint8_t green, uint8_t blue);
void IKEADIODER_HSVColorSet(uint8_t h, uint8_t s, uint8_t v);
  
  
#ifdef __cplusplus
}
#endif

#endif /* __IKEADIODER_H_ */

