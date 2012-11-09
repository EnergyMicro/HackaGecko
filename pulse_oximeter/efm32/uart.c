/**************************************************************************//**
 * @file
 * @brief USART example
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

#include "efm32.h"
#include "uart.h"
#include "efm32_gpio.h"
#include "uart_project.h"


/* Buffer pointers and indexes */
char* txBuffer;
char* rxBuffer;
int txBufferSize;
int rxBufferSize;
volatile int txBufferIndex;
volatile int rxBufferIndex;



/**************************************************************************//**
 * @brief Setup a USART to be used in asynchronous (UART) mode
 * @param usartNumber is the number of the USART to use (e.g. 1 for USART1)
 * @param location is the IO location to use
 *****************************************************************************/
void UART_setupUart(uint8_t uartNumber, uint8_t location)
{
  USART_TypeDef* uart;

  /* Determining USART */
  switch (uartNumber)
  {
  case 0:
    uart = USART0;
    break;
  case 1:
    uart = USART1;
    break;
  default:
    return;
  }


  /* Configure USART as uart - 8-N-1 */
  uart->FRAME &= ~(
    _USART_FRAME_DATABITS_MASK |
    _USART_FRAME_PARITY_MASK |
    _USART_FRAME_STOPBITS_MASK);
  uart->FRAME |= (
    USART_FRAME_DATABITS_EIGHT |
    USART_FRAME_PARITY_NONE |
    USART_FRAME_STOPBITS_ONE
    );


  /* Setting baudrate */
  uart->CLKDIV = 256 * ((UART_PERCLK_FREQUENCY / (16 * UART_BAUDRATE)) - 1);

  /* Clear RX/TX buffers */
  uart->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

  /* Enable RX/TX */
  uart->CMD = USART_CMD_RXEN | USART_CMD_TXEN;

  /* Enable RX and TX pins and set location */
  uart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC1;

  /* IO configuration */
  /*
  GPIO_PinModeSet((GPIO_Port_TypeDef) AF_PORT(AF_USART_TX(uartNumber), location),
                  AF_PIN(AF_USART_TX(uartNumber), location),
                  gpioModePushPull,0);
  GPIO_PinModeSet((GPIO_Port_TypeDef) AF_PORT(AF_USART_RX(uartNumber), location),
                  AF_PIN(AF_USART_RX(uartNumber), location),
                  gpioModePushPull,0);
  */
  GPIO_PinModeSet(3, 0, gpioModePushPull,0);
  GPIO_PinModeSet(3, 1, gpioModePushPull,0);
  
}



/**************************************************************************//**
 * @brief Setting up RX interrupt from USART1
 * @param receiveBuffer is filled with received data
 * @param receiveBufferSize is the number of bytes to receive.
 *****************************************************************************/
void UART1_setupRXInt(char* receiveBuffer, int receiveBufferSize)
{
  USART_TypeDef *uart = USART1;

  /* Setting up pointer and indexes */
  rxBuffer      = receiveBuffer;
  rxBufferSize  = receiveBufferSize;
  rxBufferIndex = 0;

  /* Clear previous interrupts */
  uart->IFC = _USART_IFC_MASK;

  /* Enable interrupts */
  NVIC_ClearPendingIRQ(USART1_RX_IRQn);
  NVIC_EnableIRQ(USART1_RX_IRQn);
  uart->IEN = USART_IEN_RXDATAV;
}



/**************************************************************************//**
 * @brief USART1 IRQ Handler
 * set up the interrupt prior to use
 *****************************************************************************/
void USART1_RX_IRQHandler(void)
{
  USART_TypeDef *uart = USART1;
  uint8_t       rxdata;

  /* Checking that RX-flag is set*/
  if (uart->STATUS & USART_STATUS_RXDATAV)
  {
    rxdata = uart->RXDATA;
    /* Store Data */
    rxBuffer[rxBufferIndex] = rxdata;
    rxBufferIndex++;

    /* Checking if the buffer is full */
    if (rxBufferIndex == rxBufferSize)
    {
      /* Disable interrupt */
      uart->IEN &= ~USART_IEN_RXDATAV;
      NVIC_ClearPendingIRQ(USART1_RX_IRQn);
      NVIC_DisableIRQ(USART1_RX_IRQn);
    }
  }
}



/******************************************************************************
 * @brief sets up and enables USART1 TX interrupt
 * @param transmitBuffer is pointing to the data to send.
 * @param bytesToSend is the number of bytes to send.
 *****************************************************************************/
void UART1_setupTXInt(char* transmitBuffer, int bytesToSend)
{
  USART_TypeDef *uart = USART1;

  /* Setting up pointer and indexes */
  txBuffer      = transmitBuffer;
  txBufferSize  = bytesToSend;
  txBufferIndex = 0;

  /* Clear previous interrupts */
  uart->IFC = _USART_IFC_MASK;
  /* Enable interrupts */
  uart->IEN = USART_IEN_TXBL;

  NVIC_ClearPendingIRQ(USART1_TX_IRQn);
  NVIC_EnableIRQ(USART1_TX_IRQn);
}



/**************************************************************************//**
 * @brief USART1 TX IRQ Handler
 * Sending until all data has been sent.
 *****************************************************************************/
void USART1_TX_IRQHandler(void)
{
  USART_TypeDef *uart = USART1;

  /* Checking that the USART is waiting for data */
  if (uart->STATUS & USART_STATUS_TXBL)
  {
    /* Transmitting the next byte */
    uart->TXDATA = txBuffer[txBufferIndex];
    txBufferIndex++;

    /* Disabling the interrupt when the buffer has been transmitted */
    if (txBufferIndex == txBufferSize)
    {
      uart->IEN &= ~USART_IEN_TXBL;
      NVIC_DisableIRQ(USART1_TX_IRQn);
      NVIC_ClearPendingIRQ(USART1_TX_IRQn);
    }
  }
}



/**************************************************************************//**
 * @brief Receiving data from USART1
 * @param receiveBuffer is pointing to where data is to be stored.
 * @param bytesToReceive is the number of bytes to receive
 *****************************************************************************/
void UART1_receiveBuffer(char* receiveBuffer, int bytesToReceive)
{
  USART_TypeDef *uart = USART1;
  int           ii;

  /* Receiving data */
  for (ii = 0; ii < bytesToReceive;  ii++)
  {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_RXDATAV)) ;

    /* Writing next byte to USART */
    *receiveBuffer = uart->RXDATA;
    receiveBuffer++;
  }
}

