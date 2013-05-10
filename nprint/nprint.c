/***************************************************************************//**
 * @file
 * @brief nprint - a very low footprint print module
 * @author Energy Micro AS
 * @version 0.00.1
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
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
/* Device specific header file(s). */
#include "em_device.h"
#include "em_int.h"

#include "em_gpio.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "nprint.h"

static void NPRINT_TxByte(uint8_t chr);

#if (NPRINT_USART_COMMS == true) 
static void NPRINT_USART_Init(void);
static void NPRINT_USART_txByte(uint8_t chr);
#endif

#if (NPRINT_SWO_COMMS == true) 
static void NPRINT_SetupSWO(void);
static uint32_t NPRINT_ITMSendChar(uint32_t ch);
#endif

/***************************************************************************//**
 * @brief
 *   Prints a zero terminated string to any enabled communication channel.
 *
 * @param[in] str
 *   Zero terminated string
 ******************************************************************************/
void NPRINT_Init(void)
{
#if (NPRINT_USART_COMMS == true) 
  NPRINT_USART_Init();
#endif  
#if (NPRINT_SWO_COMMS == true)   
  NPRINT_SetupSWO();
#endif  
}
/***************************************************************************//**
 * @brief
 *   Prints a zero terminated string to any enabled communication channel.
 *
 * @param[in] str
 *   Zero terminated string
 ******************************************************************************/
void NPRINT_Print(uint8_t * str)
{
  while (*str != 0)
  {
    NPRINT_TxByte(*str++);
  }  
}
/***************************************************************************//**
 * @brief
 *   Prints a single byte to any enabled communication channel.
 *
 * @param[in] chr
 *   byte to be printed
 ******************************************************************************/
void NPRINT_PutChar(uint8_t chr)
{
    NPRINT_TxByte(chr);  
}
/***************************************************************************//**
 * @brief
 *   converts a 4 bit value to ASCII (0-15 dec to "0"->"F" ASCII)
 *
 * @param[in] 4 bit value to be converted
 *   
 * @param[out] ASCII value ("0"->"F")
 *    
 ******************************************************************************/
uint8_t NPRINT_ConvertHexToAscii(uint8_t val)
{
  val &= 0x0F;
  return (val + ((val <= 9) ? '0' : ('A' - 10)));
}
/***************************************************************************//**
 * @brief
 *   Prints a 32 bit value as decimal ASCII. (e.g. 0x0A printed as "10"
 *
 * @param[in] val
 *   value to be printed as decimal
 ******************************************************************************/
void NPRINT_Print32BitHex(uint32_t val)
{
  uint32_t tmpVal = val;
  int8_t i;
  for (i = 7; i >= 0; i--)
  {
    NPRINT_TxByte(NPRINT_ConvertHexToAscii(tmpVal >> i * 4));
  }
}
/***************************************************************************//**
 * @brief
 *   Prints a 32 bit value as Hex ASCII. (e.g. 0x12F printed as "12A"
 *
 * @param[in] val
 *   value to be printed as decimal
 ******************************************************************************/
void NPRINT_Print32BitDec(uint32_t val)
{
  bool nonZeroFound = false;
  uint32_t digit;
  uint32_t divider = 1000000000;
  int8_t i;
  if (val != 0)
  {
    for (i=9; i >= 0;i--)
    {
      digit = val / divider;
      digit = digit % 10;
      if (nonZeroFound == false)
      {
        if (digit != 0)
        {
          nonZeroFound = true;
        }
      }
      
      if (nonZeroFound == true)
      {
        NPRINT_TxByte(NPRINT_ConvertHexToAscii(digit));
      }
      divider = divider /10;
    }
  } else
  {
    NPRINT_TxByte(NPRINT_ConvertHexToAscii(0));    
  }
}
/***************************************************************************//**
 * @brief
 *   Transmits a byte on all enabled communication interfaces
 *
 * @param[in] val
 *   value to be printed 
 ******************************************************************************/
static void NPRINT_TxByte(uint8_t chr)
{
#if (NPRINT_USART_COMMS == true) 
  NPRINT_USART_txByte(chr);  
#endif
#if (NPRINT_SWO_COMMS == true) 
  NPRINT_ITMSendChar(chr);
#endif
}
#if (NPRINT_USART_COMMS == true) 
/***************************************************************************//**
 * @brief
 *   Initializes USART based on configuration in nprint.h
 *
 ******************************************************************************/
static void NPRINT_USART_Init(void)
{
#if (NPRINT_USART_USE_EMLIB == true)  
  static USART_InitAsync_TypeDef usartInit =                                                              \
  { usartEnableTx,        /* Enable TX when init completed. */                                   \
    0,                    /* Use current configured reference clock for configuring baudrate. */ \
    NPRINT_USART_BAUDRATE,/* 115200 bits/s. */                                                   \
    usartOVS16,           /* 16x oversampling. */                                                \
    usartDatabits8,       /* 8 databits. */                                                      \
    usartNoParity,        /* No parity. */                                                       \
    usartStopbits1,       /* 1 stopbit. */                                                       \
    false,                /* Do not disable majority vote. */                                    \
    false,                /* Not USART PRS input mode. */                                        \
    usartPrsRxCh0         /* PRS channel 0. */                                                   \
  };  
#endif  
  /*enable clocks that is required..*/
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(NPRINT_USART_CMUCLOCK, true);
  
  /*Setup GPIO pins used by USART selected*/
  GPIO_PinModeSet((GPIO_Port_TypeDef)NPRINT_USART_TX_PORT, NPRINT_USART_TX_PIN, gpioModePushPull, 1);
  GPIO_PinModeSet((GPIO_Port_TypeDef)NPRINT_USART_RX_PORT, NPRINT_USART_RX_PIN, gpioModeInput, 0);
#if (NPRINT_USART_USE_EMLIB == true)  
  usartInit.baudrate = NPRINT_USART_BAUDRATE;
  
   /* Initialize USART with uartInit struct */
  USART_InitAsync(NPRINT_USART_USED, &usartInit);
#else
  /* Set the clock division */
  NPRINT_USART_USED->CLKDIV = NPRINT_USART_CLKDIV;
  /* Clear buffers */
  NPRINT_USART_USED->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
  /* Enable TX */
  NPRINT_USART_USED->CMD =  USART_CMD_TXEN;
  
  
#endif  
  /* Enable I/O pins at UART1 location #2 */
  NPRINT_USART_USED->ROUTE = UART_ROUTE_RXPEN | UART_ROUTE_TXPEN | NPRINT_USART_LOCATION;  
}

/***************************************************************************//**
 * @brief
 *   Prints a character to the USART
 *
 * @param[in] chr
 *   value to be printed
 ******************************************************************************/
static void NPRINT_USART_txByte(uint8_t chr)
{
  /* Check that transmit buffer is empty */
  while (!(NPRINT_USART_USED->STATUS & USART_STATUS_TXBL)) ;
  NPRINT_USART_USED->TXDATA = (uint32_t) chr;
}
#endif

#if (NPRINT_SWO_COMMS == true) 
/***************************************************************************//**
 * @brief
 *   Sets up single wire output on the port selected in nprint.h
 *
 ******************************************************************************/
static void NPRINT_SetupSWO(void)
{
  uint32_t *dwt_ctrl = (uint32_t *) 0xE0001000;
  uint32_t *tpiu_prescaler = (uint32_t *) 0xE0040010;
  uint32_t *tpiu_protocol = (uint32_t *) 0xE00400F0;
  uint32_t *tpiu_ffcr = (uint32_t *) 0xE0040304;

  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

  /* Enable Serial wire output pin */
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

#if defined(_EFM32_GECKO_FAMILY) || defined(_EFM32_TINY_FAMILY)
  /* Set location 1 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC1;
  /* Enable output on pin */
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#elif defined(_EFM32_GIANT_FAMILY)
  /* Set location 0 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;
  /* Enable output on pin */
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
  #error Unknown device family!
#endif

  /* Enable debug clock AUXHFRCO */
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  while(!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

  /* Enable trace in core debug */
  CoreDebug->DHCSR |= 1;
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  /* Enable PC and IRQ sampling output */
  *dwt_ctrl = 0x400113FF;

  /* Set TPIU prescaler to 16 (14 MHz / 16 = 875 kHz SWO speed) */
  *tpiu_prescaler = 0xf;

  /* Set protocol to NRZ */
  *tpiu_protocol = 2;
  *tpiu_ffcr = 0x100;

  /* Unlock ITM and output data */
  ITM->LAR = 0xC5ACCE55;
  ITM->TCR = 0x10009;

  /* ITM Channel 1 is used for UART output */

  ITM->TER |= (1UL << NPRINT_SWO_PORT_USED);    
}
/***************************************************************************//**
 * @brief
 *   Prints a character to the SWO port selected in nprint.h
 *
 * @param[in] chr
 *   value to be printed
 ******************************************************************************/
static uint32_t NPRINT_ITMSendChar(uint32_t ch)
{
  if ((ITM->TCR & ITM_TCR_ITMENA_Msk) &&      /* ITM enabled */
      (ITM->TER & (1UL << NPRINT_SWO_PORT_USED)))                /* ITM Port #1 enabled */
  {
    while (ITM->PORT[NPRINT_SWO_PORT_USED].u32 == 0);
    ITM->PORT[NPRINT_SWO_PORT_USED].u8 = (uint8_t) ch;
  }
  return (ch);
}
#endif