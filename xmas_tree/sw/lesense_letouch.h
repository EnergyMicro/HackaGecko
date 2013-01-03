/**************************************************************************//**
 * @file lesense_letouch.h
 * @brief LESENSE CAPACITIVE TOUCH code example
 * @author Energy Micro AS
 * @version 2.02
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
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

#ifndef __LESENSE_LETOUCH_H
#define __LESENSE_LETOUCH_H

/**************************************************************************//**
 * Interrupt handlers prototypes
 *****************************************************************************/
void LESENSE_IRQHandler(void);

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
void LETOUCH_Init(float sensitivity[]);
uint16_t LETOUCH_GetChannelsTouched(void);
uint16_t LETOUCH_GetChannelMaxValue(uint8_t channel);
uint16_t LETOUCH_GetChannelMinValue(uint8_t channel);
void LETOUCH_Calibration(void);

#endif /* __LESENSE_LETOUCH_H */
