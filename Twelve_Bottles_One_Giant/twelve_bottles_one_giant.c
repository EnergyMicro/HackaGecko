/**************************************************************************//**
 * @file twelve_bottles_one_giant.c
 * @brief twelve_bottles_one_giant.c
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "em_device.h"
#include "em_cmu.h"
#include "bsp.h"
#include "rtc.h"
#include "bsp_trace.h"
#include "tftamapped.h"
#include "tftdraw.h"
#include "glib/glib.h"

#include "em_adc.h"
#include "em_int.h"
#include "em_emu.h"
#include "em_rtc.h"



/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define NUM_SAMPLES 8
#define NUMBER_OF_KASSES 1
#define BEERS_IN_KASSE 12
#define BEER_SIZE 30 //Size of beer in pixels. Should be divisible with 2.
#define BEER_DISPLACEMENT 30 //Distance from Edge of case


#define LFXO_FREQUENCY         32768
#define RTC_TIMEOUT_S          5
#define RTC_COUNT_TO_WAKEUP    (LFXO_FREQUENCY * RTC_TIMEOUT_S)

/** Graphics context */
GLIB_Context    gc;

/** Counts 1ms timeTicks */
volatile uint32_t msTicks;

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
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{

  bool     redraw = false;
  EMSTATUS status;
 

  /* Use 28MHZ HFRCO as core clock frequency */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
  CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);


  /* Initialize DVK board register access */
  /* This demo currently only works in EBI mode */
  /* Initialize DK board register access */
  BSP_Init(BSP_INIT_DEFAULT);

  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();


  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000))
  {
    while (1) ;
  }

  /* Wait until we have control over display */
  while(!redraw)
  {
    redraw = TFT_AddressMappedInit();
  }

  /* Init graphics context - abort on failure */
  status = GLIB_contextInit(&gc);
  if (status != GLIB_OK) while (1) ;

  /* Enable clocks required */
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_ADC0, true);
  CMU_ClockEnable(cmuClock_CORELE, true);
  __NOP();
  __NOP();
  CMU_ClockEnable(cmuClock_RTC, true);

  ADC0->SINGLECTRL = ADC_SINGLECTRL_REF_2V5;
  
  uint32_t samples[NUM_SAMPLES];
  uint32_t sample;
  while(1) {
    
    for (int i=0; i<NUM_SAMPLES; i++)
    {
      ADC0->SINGLECTRL = (ADC_SINGLECTRL_INPUTSEL_CH0 + i) << _ADC_SINGLECTRL_INPUTSEL_SHIFT | ADC_SINGLECTRL_AT_32CYCLES;
      
      ADC0->CMD = ADC_CMD_SINGLESTART;
      
      /* Wait while conversion is active */
      while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;
      
      /* Get ADC result */
      sample = ADC_DataSingleGet(ADC0);
      /* Populate sample array */
      
      samples[i] = sample;
    }
  
    /* Check if we should control TFT display instead of AEM/board controller */
    bool redraw = TFT_AddressMappedInit();
    if(redraw)
    {
      /* Update display */
      TFT_displayUpdate(&gc, samples, NUM_SAMPLES);
    }
    else
    {
      /* No need to refresh display when BC is active */
      Delay(200);
    }
    
    RTC_Trigger(2000, NULL);
    EMU_EnterEM2(false);
  }
}
