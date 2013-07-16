/**************************************************************************//**
 * @file gps_module.c
 * @brief GPS-logger - GPS Module Handler
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
   
/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/
 
/* Define termination character */
#define TERMINATION_CHAR   0x0A

/* Declare a circular buffer structure to use for Rx and Tx queues */
#define BUFFERSIZE          512
   
/* Declare a minimum number of bytes saved on SDCARD */   
#define MIN_BYTES_COUNT     100
   
/*******************************************************************************
 ******************************   STATICS   ************************************
 ******************************************************************************/
   
/* USART structs */   
static USART_TypeDef           *usart   = USART0;
static USART_InitAsync_TypeDef usartInit = USART_INITASYNC_DEFAULT;

/*******************************************************************************
 ******************************   STRUCTS   ************************************
 ******************************************************************************/

volatile struct circularBuffer
{
  uint8_t  data[BUFFERSIZE];  /* data buffer */
  uint32_t rdI;               /* read index */
  uint32_t wrI;               /* write index */
  uint32_t pendingBytes;      /* count of how many bytes are not yet handled */
  bool     overflow;          /* buffer overflow indicator */
} rxBuf, txBuf = { {0}, 0, 0, 0, false };

/*******************************************************************************
 *************************   LOCAL PROTOTYPES   ********************************
 ******************************************************************************/

void usartPutData(uint8_t * dataPtr, uint32_t dataLen);
uint32_t usartGetData(uint8_t * dataPtr, uint32_t dataLen);


/***************************************************************************//**
 * @fn GPS_Initialization
 * @brief Initialize GPS Module
 *******************************************************************************/
void GPS_Initialization()
{    
   volatile int waiting;
   
  /* Configure GPIO pins */
  GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 1);
  GPIO_PinModeSet(gpioPortC, 1, gpioModeInput, 0);
  
  /* Prepare struct for initializing UART in asynchronous mode*/
  usartInit.enable       = usartDisable;   /* Don't enable UART upon intialization */
  usartInit.refFreq      = 0;              /* Provide information on reference frequency. When set to 0, the reference frequency is */
  usartInit.baudrate     = 9600;           /* Baud rate */
  usartInit.oversampling = usartOVS16;     /* Oversampling. Range is 4x, 6x, 8x or 16x */
  usartInit.databits     = usartDatabits8; /* Number of data bits. Range is 4 to 10 */
  usartInit.parity       = usartNoParity;  /* Parity mode */
  usartInit.stopbits     = usartStopbits1; /* Number of stop bits. Range is 0 to 2 */
  usartInit.mvdis        = false;          /* Disable majority voting */
  usartInit.prsRxEnable  = false;          /* Enable USART Rx via Peripheral Reflex System */
  
  /* Initialize USART with usartInit struct */
  USART_InitAsync(usart, &usartInit);
  
  /* Prepare USART Rx and Tx interrupts */
  USART_IntClear(usart, _USART_IF_MASK);
  NVIC_ClearPendingIRQ(USART0_RX_IRQn);
  NVIC_ClearPendingIRQ(USART0_TX_IRQn);
  NVIC_EnableIRQ(USART0_TX_IRQn);
  
  /* Enable I/O pins at UART1 location #5 */
  usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC5;
  
  /* Enable UART */
  USART_Enable(usart, usartEnableTx);
  
  /* Send configuration packet 
  *
  *  Configuration:
  *    - 5 second interval between GPS packet
  *    - set output format (GGA)
  */
  usartPutData("$PMTK314,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*2D", 50);
  uint8_t cmd_init[2] = {0x0d, 0x0a};
  usartPutData(&cmd_init[0], 1);
  usartPutData(&cmd_init[1], 1);  
  
  /* Wait a moment */
  Delay(5000);
  
  /* Configuration COLD Start of GPS */
  usartPutData("$PMTK103*30", 11);
  usartPutData(&cmd_init[0], 1);
  usartPutData(&cmd_init[1], 1);  
  
  /* Wait a moment */
  Delay(5000);
  
  /* Enable all needed interrupts */
  NVIC_ClearPendingIRQ(USART0_RX_IRQn);
  USART_IntEnable(usart, USART_IF_RXDATAV);
  NVIC_EnableIRQ(USART0_RX_IRQn);
  USART_Enable(usart, usartEnable);
  
}

/***************************************************************************//**
 * @fn GPS_WaitForData
 * @brief Watiting for GPS data
 *******************************************************************************/
int GPS_WaitForData(sGPS_Data* data)
{
  while(1)
  {
    /* Wait in EM1 while UART transmits */
    EMU_EnterEM1();
    
    /* Check if termination character is received (save if is more than MIN_BYTES_COUNT) */
    if (rxBuf.pendingBytes >= MIN_BYTES_COUNT)
    {
      if(rxBuf.data[rxBuf.wrI - 1] == TERMINATION_CHAR)
      {        
       /* Disable IRQ while saving */
       NVIC_DisableIRQ(USART0_RX_IRQn);
          
       /* Get actual data */
       int len = usartGetData(data->string, 0); 
       data->length = len;
       
       /* Re-enable IRQ */
       NVIC_EnableIRQ(USART0_RX_IRQn);
       
       return 1;
      }
    }  
  }
}

/***************************************************************************//**
 * @fn usartPutData
 * @brief Send data to GPS module
 *******************************************************************************/
void usartPutData(uint8_t * dataPtr, uint32_t dataLen)
{
  uint32_t i = 0;

  /* Check if buffer is large enough for data */
  if (dataLen > BUFFERSIZE)
  {
    /* Buffer can never fit the requested amount of data */
    return;
  }

  /* Check if buffer has room for new data */
  if ((txBuf.pendingBytes + dataLen) > BUFFERSIZE)
  {
    /* Wait until room */
    while ((txBuf.pendingBytes + dataLen) > BUFFERSIZE) ;
  }

  /* Fill dataPtr[0:dataLen-1] into txBuffer */
  while (i < dataLen)
  {
    txBuf.data[txBuf.wrI] = *(dataPtr + i);
    txBuf.wrI             = (txBuf.wrI + 1) % BUFFERSIZE;
    i++;
  }

  /* Increment pending byte counter */
  txBuf.pendingBytes += dataLen;

  /* Enable interrupt on USART TX Buffer*/
  USART_IntEnable(usart, USART_IF_TXBL);
}

/***************************************************************************//**
 * @fn usartGetData
 * @brief Get data from GPS module
 *******************************************************************************/
uint32_t usartGetData(uint8_t * dataPtr, uint32_t dataLen)
{
  uint32_t i = 0;

  /* Wait until the requested number of bytes are available */
  if (rxBuf.pendingBytes < dataLen)
  {
    while (rxBuf.pendingBytes < dataLen) ;
  }

  if (dataLen == 0)
  {
    dataLen = rxBuf.pendingBytes;
  }

  /* Copy data from Rx buffer to dataPtr */
  while (i < dataLen)
  {
    *(dataPtr + i) = rxBuf.data[rxBuf.rdI];
    rxBuf.rdI      = (rxBuf.rdI + 1) % BUFFERSIZE;
    i++;
  }

  /* Decrement pending byte counter */
  rxBuf.pendingBytes -= dataLen;

  return i;
}

/***************************************************************************//**
 * @fn USART0_RX_IRQHandler
 * @brief USART0 RX Interrupt handler
 *******************************************************************************/
void USART0_RX_IRQHandler(void)
{
  /* Check for RX data valid interrupt */
  if (usart->STATUS & USART_STATUS_RXDATAV)
  {
    /* Copy data into RX Buffer */
    uint8_t rxData = USART_Rx(usart);
    rxBuf.data[rxBuf.wrI] = rxData;
    rxBuf.wrI             = (rxBuf.wrI + 1) % BUFFERSIZE;
    rxBuf.pendingBytes++;

    /* Flag Rx overflow */
    if (rxBuf.pendingBytes > BUFFERSIZE)
    {
      rxBuf.overflow = true;
    }

    /* Clear RXDATAV interrupt */
    USART_IntClear(usart, USART_IF_RXDATAV);
  }
}

/***************************************************************************//**
 * @fn USART0_TX_IRQHandler
 * @brief USART0 TX Interrupt handler
 *******************************************************************************/
void USART0_TX_IRQHandler(void)
{
  /* Clear interrupt flags by reading them. */
  USART_IntGet(USART0);

  /* Check TX buffer level status */
  if (usart->STATUS & USART_STATUS_TXBL)
  {
    if (txBuf.pendingBytes > 0)
    {
      /* Transmit pending character */
      USART_Tx(usart, txBuf.data[txBuf.rdI]);
      txBuf.rdI = (txBuf.rdI + 1) % BUFFERSIZE;
      txBuf.pendingBytes--;
    }

    /* Disable Tx interrupt if no more bytes in queue */
    if (txBuf.pendingBytes == 0)
    {
      USART_IntDisable(usart, USART_IF_TXBL);
    }
  }
}