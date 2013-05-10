/***************************************************************************//**
 * @file
 * @brief nprint - a very low footprint print module header file
 * @author Energy Micro AS
 * @version 0.00.1
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

#ifndef __NPRINT_H
#define __NPRINT_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


/*Set one to true or false to enable different communication interfaces */
#define NPRINT_USART_COMMS true
#define NPRINT_SWO_COMMS   false
    
/*USART configuration - if USART number is changed, remember to change all 
  macros below containing USART1...(6 in total)*/  
#define NPRINT_USART_USE_EMLIB      false
/*Divider of 0x1a - 115200 baud at 14 MHZ - only used if NPRINT_USART_USE_EMLIB
  is false */  
#define NPRINT_USART_CLKDIV         0x680
/*baudrate - only used if #define NPRINT_USART_BAUDRATE  115200 is true*/
#define NPRINT_USART_BAUDRATE  115200  

/* General usart settings */ 
#define NPRINT_USART_USED      USART1
#define NPRINT_USART_LOCATION  USART_ROUTE_LOCATION_LOC1
#define NPRINT_USART_CMUCLOCK  cmuClock_USART1
#define NPRINT_USART_TX_PORT         AF_USART1_TX_PORT(NPRINT_USART_LOCATION >> _USART_ROUTE_LOCATION_SHIFT)
#define NPRINT_USART_TX_PIN          AF_USART1_TX_PIN(NPRINT_USART_LOCATION >> _USART_ROUTE_LOCATION_SHIFT)
#define NPRINT_USART_RX_PORT         AF_USART1_RX_PORT(NPRINT_USART_LOCATION >> _USART_ROUTE_LOCATION_SHIFT)
#define NPRINT_USART_RX_PIN          AF_USART1_RX_PIN(NPRINT_USART_LOCATION >> _USART_ROUTE_LOCATION_SHIFT)

  
/* SWO configuration */
#define NPRINT_SWO_PORT_USED 1  

/***************************************************************************//**
 * @brief
 *   Initializes NPRINT module based on configuration above.
 *
 ******************************************************************************/    
void NPRINT_Init(void);
/***************************************************************************//**
 * @brief
 *   Prints a zero terminated string to any enabled communication channel.
 *
 * @param[in] str
 *   Zero terminated string
 ******************************************************************************/
void NPRINT_Print(uint8_t * str);
/***************************************************************************//**
 * @brief
 *   Prints a single byte to any enabled communication channel.
 *
 * @param[in] chr
 *   byte to be printed
 ******************************************************************************/
void NPRINT_Putchar(uint8_t chr);
/***************************************************************************//**
 * @brief
 *   converts a 4 bit value to ASCII (0-15 dec to "0"->"F" ASCII)
 *
 * @param[in] 4 bit value to be converted
 *   
 * @param[out] ASCII value ("0"->"F")
 *    
 ******************************************************************************/
uint8_t NPRINT_ConvertHexToAscii(uint8_t val);
/***************************************************************************//**
 * @brief
 *   Prints a 32 bit value as decimal ASCII. (e.g. 0x0A printed as "10"
 *
 * @param[in] val
 *   value to be printed as decimal
 ******************************************************************************/
void NPRINT_Print32BitDec(uint32_t val);
/***************************************************************************//**
 * @brief
 *   Prints a 32 bit value as Hex ASCII. (e.g. 0x12F printed as "12A"
 *
 * @param[in] val
 *   value to be printed as decimal
 ******************************************************************************/
void NPRINT_Print32BitHex(uint32_t val);
#ifdef __cplusplus
}
#endif

#endif
