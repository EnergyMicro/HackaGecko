/******************************************************************************
 * @file
 * @brief Uart Demo Application
 * @author Energy Micro AS
 * @version 1.04
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
#include <string.h>
#include "efm32.h"
#include "efm32_chip.h"
#include "efm32_cmu.h"
#include "uart.h"
#include "usart.h"


/* Buffers */
char transmitBuffer[] = "Energy Micro";
#define            BUFFERSIZE    (sizeof(transmitBuffer) / sizeof(char))
char receiveBuffer[BUFFERSIZE];



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



/******************************************************************************
 * @brief initializing uart settings
 *
 *****************************************************************************/
void init(void)
{
  /* Using HFXO to have max frequency accuracy */
  switchToHFXO();

  /* Enabling clock to USART 1 and 2*/
  CMU_ClockEnable(cmuClock_USART1, true);
  CMU_ClockEnable(cmuClock_USART2, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  
  /* Setup UART */
  UART_setupUart(USART1_NUM, GPIO_POS1);
  UART_setupUart(USART2_NUM, GPIO_POS0);
}



/******************************************************************************
 * @brief  Main function
 * Main is called from _program_start, see assembly startup file
 *****************************************************************************/
int main(void)
{
  /* Initialize chip */
  CHIP_Init();
  
  /* Initalizing */
  init();

  /* Transmitting data */
  /* Reception is interrupt driven */
  /* Transmission is polled */
  UART1_setupRXInt(receiveBuffer, BUFFERSIZE);
  USART2_sendBuffer(transmitBuffer, BUFFERSIZE);

  /* Clearing the receive buffer */
  memset(receiveBuffer, '\0', BUFFERSIZE);

  /* Transmitting data */
  /* Transmission is interrupt driven */
  /* Reception is polled */
  UART2_setupTXInt(transmitBuffer, BUFFERSIZE);
  UART1_receiveBuffer(receiveBuffer, BUFFERSIZE);

  /* Done! */
  while (1) ;
}



