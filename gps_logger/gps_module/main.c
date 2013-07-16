/**************************************************************************//**
 * @file main.c
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

#include <stdint.h>
#include <stdbool.h>

#include "app.h"
   
sGPS_Data gpsData;

/* Counts 1ms timeTicks */
volatile uint32_t msTicks; 

/**************************************************************************//**
 * @brief SysTick_Handler
 *   Interrupt Service Routine for system tick counter
 * @note
 *   No wrap around protection
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++; /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

/***************************************************************************//**
 * @fn LCD_Show
 * @brief Indicate positive saved data on SDCARD
 *******************************************************************************/
void LCD_Show()
{
    SegmentLCD_Write("GOT!");
    Delay(200);
    SegmentLCD_AllOff(); 
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();
  
    /* Use 32MHZ HFXO as core clock frequency*/
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  
  /* Enable clock for HF peripherals */
  CMU_ClockEnable(cmuClock_HFPER, true);
  
  /* Enabling clock to USART0*/
  CMU_ClockEnable(cmuClock_USART0, true);
  
  /* Enable clock for GPIO module (required for pin configuration) */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;

  /* Enable LCD without voltage boost */
  SegmentLCD_Init(false);
  
  /* Enable all segments */
  SegmentLCD_AllOff(); 
  
  SegmentLCD_Write("GPS!");
  Delay(2000);
  SegmentLCD_AllOff(); 

  /* Initialization of GPS */
  GPS_Initialization();
  
  SegmentLCD_Write("SD!");
  Delay(2000);
  SegmentLCD_AllOff(); 
  
  /* Initialization of SDCARD */
  SDCARD_Initialization();
   
  SegmentLCD_AllOff();
  SegmentLCD_Write("DONE!");
  Delay(2000);
  
  SegmentLCD_AllOff();
  
  /* Infinite loop with test pattern. */
  while (1)
  {
    /* Wait for data in sleep mode */
    GPS_WaitForData(&gpsData);
     
    /* Save it on SDCARD */
    SDCARD_Save(&gpsData);
     
    /* Indicate on LCD */
    LCD_Show();
  }
}
