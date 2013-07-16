/**************************************************************************//**
 * @file app.h
 * @brief GPS-logger
 * @author Energy Micro AS
 * @version 1.0.2
 ******************************************************************************
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
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
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
 *****************************************************************************/

#ifndef __APP_H
#define __APP_H

#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "bsp.h"
#include "segmentlcd.h"
#include "bsp_trace.h"

/*******************************************************************************
 **************************   GLOBAL STRUCTS   *********************************
 ******************************************************************************/

/* GPS prototypes */
typedef struct{
  uint8_t string[1024];
  uint8_t length;
} sGPS_Data;

/*******************************************************************************
 *************************   GLOBAL VARIABLES   ********************************
 ******************************************************************************/

/* Counts 1ms timeTicks */
extern volatile uint32_t msTicks;

/*******************************************************************************
 ******************************   FUNCTIONS  ***********************************
 ******************************************************************************/
/* Simple delay on SysTick */
void Delay(uint32_t dlyTicks);

/* GPS functions */
void GPS_Initialization();
int GPS_WaitForData(sGPS_Data* data);

/* SDCARD function */
void SDCARD_Initialization();                    
void SDCARD_Save(sGPS_Data* data);

#endif