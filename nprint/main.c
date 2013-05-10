/**************************************************************************//**
 * @file
 * @brief nprint example for EFM32GG_STK3700
 * @author Energy Micro AS
 * @version 3.20.0
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
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "bsp.h"
#include "bsp_trace.h"
#include "nprint.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t dlyTicks);
/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
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
/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();
  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();
  /* Setup SysTick Timer for 1 msec interrupts  */
  
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;
  
  NPRINT_Init();
  /* Initialize LED driver */
  BSP_LedsInit();
  BSP_LedSet(0);


  NPRINT_Print("hello world!\r\n");
  NPRINT_Print("test 1: hex val 0x");
  NPRINT_Print32BitHex(0x1289ABEF);
  NPRINT_Print("\r\n");
  NPRINT_Print("test 2: hex val 0x");
  NPRINT_Print32BitHex(0);
  NPRINT_Print("\r\n");
  NPRINT_Print("test 3: dec val ");
  NPRINT_Print32BitDec(112204450);
  NPRINT_Print("\r\n");
  NPRINT_Print("test 4: dec val ");
  NPRINT_Print32BitDec(0);
  NPRINT_Print("\r\n");
  NPRINT_Print("test 5: dec val ");
  NPRINT_Print32BitDec(0xFFFFFFFF);
  NPRINT_Print("\r\n");
  
  /* Infinite blink loop */
  while (1)
  {
    BSP_LedToggle(0);
    BSP_LedToggle(1);
    Delay(1000);
  }
}
