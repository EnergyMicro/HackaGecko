/**************************************************************************//**
 * @file
 * @brief OPA1 connected to the ADC.
 * @author Energy Micro AS
* @version 1.01
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2009 Energy Micro AS, http://www.energymicro.com</b>
 ******************************************************************************
 *
 * This source code is the property of Energy Micro AS. The source and compiled
 * code may only be used on Energy Micro "EFM32" microcontrollers.
 *
 * This copyright notice may not be removed from the source code nor changed.
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
#include <stdio.h>
#include <string.h>
#include "efm32.h"
#include "efm32_chip.h"
#include "efm32_opamp.h"
#include "efm32_cmu.h"
#include "efm32_adc.h"
#include "efm32_dac.h"
#include "segmentlcd.h"
#include "uart.h"
#include "usart.h"


/******************************************************************************
 * @brief enables high frequency crystal oscillator (HFXO),
 * Sets HFCLK domain to use HFXO as source
 *****************************************************************************/
void switchToHFXO(void)
{
  CMU_TypeDef *cmu = CMU;

  /* Turning on HFXO to increase frequency accuracy. */
  /* Waiting until oscillator is stable */
  cmu->OSCENCMD = CMU_OSCENCMD_HFXOEN;
  while (!(cmu->STATUS && CMU_STATUS_HFXORDY)) ;

  /* Switching the CPU clock source to HFXO */
  cmu->CMD = CMU_CMD_HFCLKSEL_HFXO;

  /* Turning off the high frequency RC Oscillator (HFRCO) */
  /* GENERATL WARNING! Make sure not to disable the current
   * source of the HFCLK. */
  cmu->OSCENCMD = CMU_OSCENCMD_HFRCODIS;
}

static void ADCConfig(void)
{
    ADC_Init_TypeDef     init     = ADC_INIT_DEFAULT;
    ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

    init.timebase = ADC_TimebaseCalc(0);
    init.prescale = ADC_PrescaleCalc(7000000, 0);
    ADC_Init(ADC0, &init);

    singleInit.reference = adcRef2V5;
    singleInit.input     = adcSingleInpCh5;
 
    ADC_InitSingle(ADC0, &singleInit);
}


void DAC_setup(void)
{
  /* Define DAC settings */
  DAC_Init_TypeDef init =  
  { 
    dacRefresh8,              /* Refresh every 8 prescaled cycles. */    \
    dacRef1V25,               /* 1.25V internal reference. */            \
    dacOutputPinADC,          /* Output to pin only. */                  \
    dacConvModeContinuous,    /* Continuous mode. */                     \
    0,                        /* No prescaling. */                       \
    false,                    /* Do not enable low pass filter. */       \
    false,                    /* Do not reset prescaler on ch0 start. */ \
    false,                    /* DAC output enable always on. */         \
    false,                    /* Disable sine mode. */                   \
    false                     /* Single ended mode. */                   \
  };
  
  DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;

  /* Calculate the DAC clock prescaler value that will result in a DAC clock
   * close to 500kHz. Second parameter is zero, if the HFPERCLK value is 0, the
   * function will check what the current value actually is. */
  init.prescale = DAC_PrescaleCalc(500000, 0);

  /* Initialize the DAC and DAC channel. */
  DAC_Init(DAC0, &init);
  
  /*Initialize DAC channel 0.*/
  DAC_InitChannel(DAC0, &initChannel, 0);

}

int main(void)
{ 
    CHIP_Init();
    
    /*Initilize LCD*/
    SegmentLCD_Init(false);
  
    /* Using HFXO to have max frequency accuracy */
    switchToHFXO();

    /*Enable clocks*/
    CMU_ClockEnable(cmuClock_DAC0, true);
    CMU_ClockEnable(cmuClock_ADC0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART1, true);
  
    /* Setup UART */
    UART_setupUart(USART1_NUM, GPIO_POS1);
    
    /*Configure ADC*/
    ADCConfig();
              
    /*Configure OPA2 and enable*/
    DAC0->OPA2MUX |= DAC_OPA2MUX_OUTMODE | DAC_OPA2MUX_OUTPEN_OUT0 | DAC_OPA2MUX_PPEN | DAC_OPA2MUX_NPEN | DAC_OPA2MUX_NEGSEL_NEGPAD | DAC_OPA2MUX_POSSEL_POSPAD;
    DAC0->OPACTRL |= DAC_OPACTRL_OPA2EN;

    /*Configure DAC and enable*/
//    DAC0->CH0CTRL |= DAC_CH0CTRL_REFREN | DAC_CH0CTRL_EN;
//    DAC0->CH0DATA = (uint32_t)((0.136 * 4096) / 1.25);    
          
    /*Configure OPA0 and enable*/  
//    DAC0->CTRL |= DAC_CTRL_OUTMODE_DISABLE;
//    DAC0->OPA0MUX |= DAC_OPA0MUX_OUTMODE_MAIN | DAC_OPA0MUX_NPEN | DAC_OPA0MUX_NEGSEL_NEGPAD | DAC_OPA0MUX_POSSEL_DAC;
//    DAC0->OPACTRL |= DAC_OPACTRL_OPA0EN;

    
    /* Enable LED outputs*/
    GPIO->P[3].MODEL |= GPIO_P_MODEL_MODE6_PUSHPULL;
    GPIO->P[3].MODEH |= GPIO_P_MODEH_MODE8_PUSHPULL;

    /* Turn off IR LED*/
    GPIO->P[3].DOUTSET |= (0x1UL << 8);
    /* Turn on red LED*/
    GPIO->P[3].DOUTCLR |= (0x1UL << 6);

    /* Turn on IR LED*/
//    GPIO->P[3].DOUTCLR |= (0x1UL << 8);
    /* Turn off red LED*/
//    GPIO->P[3].DOUTSET |= (0x1UL << 6);
    
    /*While loop that get data from ADC and displays it on the LCD screen*/
    while (1)
    {
        /*Start ADC*/
        ADC_Start(ADC0, adcStartSingle);
        
        /*Wait until the ADC has warmed up*/
        while (ADC0->STATUS & ADC_STATUS_SINGLEACT);

        /*Get data from ADC*/
        int ADC_data = ADC_DataSingleGet(ADC0);
        double voltage = ((double) ADC_data)*3.3/4096;
        
        /*Write to LCD*/
        char buffer[10];
        sprintf(buffer, "%1.3f", voltage);
        SegmentLCD_Write(buffer);
        sprintf(buffer, "%1.3f\n", voltage);
        USART1_sendBuffer(buffer, 6);
    } 
}
