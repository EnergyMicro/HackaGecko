/**************************************************************************//**
 * @file main.c
 * @brief USB HID keyboard device example.
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
#include "em_device.h"
#include "em_usb.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "segmentlcd.h"
#include "bsp_trace.h"
#include "stdlib.h"
/**************************************************************************//**
 *
 * This example shows how a HID keyboard can be implemented.
 *
 *****************************************************************************/

/*** Typedef's and defines. ***/
#define POLL_TIMER              0
#define IDLE_TIMER              1
#define DEFAULT_POLL_TIMEOUT    24 /*in MilliSeconds*/
#define DEFAULT_IDLE_RATE       0 /*in MilliSeconds*/
#define TEST_TIMEROUT              100
#define HEARTBEAT_MASK          0xF
#define KEYLED_MASK             0x8000
#define KBDLED_MASK             0xF00
#define INTR_IN_EP_ADDR         0x81
#define ACTIVITY_LED            gpioPortE,2 /* The blue led labeled STATUS. */


/*Button pin definitions - all reside on Port D*/
#define BUTTON0                 4
#define BUTTON1                 5
#define BUTTON2                 6
#define BUTTON3                 7
#define BUTTON4                 14
#define BUTTON5                 13

/*Joystick pin definitions*/
#define JOYSTICKLEFT               2
#define JOYSTICKUP                 0
#define JOYSTICKDOWN               1
#define JOYSTICKRIGHT              3

/*** Function prototypes. ***/

static int  OutputReportReceived(USB_Status_TypeDef status,
                                 uint32_t xferred,
                                 uint32_t remaining);
static int  SetupCmd(const USB_Setup_TypeDef *setup);
static void StateChange(USBD_State_TypeDef oldState,
                        USBD_State_TypeDef newState);

static void IdleTimeout(void);

/*** Include device descriptor definitions. ***/

#include "descriptors.h"

/*** Variables ***/

//static int      keySeqNo;           /* Current position in report table */
static uint16_t  idleRate;             /*  */
static uint32_t tmpBuffer;
static volatile gamepadReport_t gamepadReport;
static volatile gamepadReport_t lastGamepadReport;

/**************************************************************************//**
 * @brief main - the entrypoint after reset.
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();

  /* If first word of user data page is non-zero, enable eA Profiler trace */
  BSP_TraceProfilerSetup();

  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIO_PinModeSet(gpioPortD,BUTTON0, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,BUTTON1, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,BUTTON2, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,BUTTON3, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,BUTTON4, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,BUTTON5, gpioModeInputPull, 1);

  
  GPIO_PinModeSet(gpioPortD,JOYSTICKUP, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,JOYSTICKDOWN, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,JOYSTICKLEFT, gpioModeInputPull, 1);
  GPIO_PinModeSet(gpioPortD,JOYSTICKRIGHT, gpioModeInputPull, 1);



  GPIO_PinModeSet(ACTIVITY_LED, gpioModePushPull,  0);
  CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );
  SegmentLCD_Init(false);
  SegmentLCD_Write("usb pad");
  SegmentLCD_Symbol(LCD_SYMBOL_GECKO, true);


  
  USBD_Init(&initstruct);
  /*
   * When using a debugger it is practical to uncomment the following three
   * lines to force host to re-enumerate the device.
   */
  /* USBD_Disconnect();      */
  /* USBTIMER_DelayMs(1000); */
  /* USBD_Connect();         */

  for (;;)
  {
  }
}

/**************************************************************************//**
 * @brief
 *   Called on timer elapsed event. This function is called at poll rate.
 *   it scans the joystick and buttons and sends a report if there is a change.
 *   If the idle timer is active (idle rate != o) the idle timer is restarted.
 ******************************************************************************/
static void PollTimeout(void)
{
  uint32_t portVal;
  gamepadReport.buttons = 0;
  gamepadReport.leftX = 0;
  gamepadReport.leftY = 0;
//  gamepadReport.rightX = 0;
//  gamepadReport.rightY = 0;  
   portVal = GPIO_PortInGet(gpioPortD);
  /* Check pushbutton */
  if ((portVal & (1<<BUTTON0)) == 0)
  {
    gamepadReport.buttons |= 1;
  }
  if ((portVal & (1<<BUTTON1)) == 0)
  {
    gamepadReport.buttons |= 2;
  }
  if ((portVal & (1<<BUTTON2)) == 0)
  {
    gamepadReport.buttons |= 4;
  }
  if ((portVal & (1<<BUTTON3)) == 0)
  {
    gamepadReport.buttons |= 8;
  }
  if ((portVal & (1<<BUTTON4)) == 0)
  {
    gamepadReport.buttons |= 0x10;
  }
  if ((portVal & (1<<BUTTON5)) == 0)
  {
    gamepadReport.buttons |= 0x20;
  }
    
  if ((portVal & (1<<JOYSTICKUP)) == 0)
  {
    gamepadReport.leftY = -127;      
  }
  if ((portVal & (1<<JOYSTICKDOWN)) == 0)
  {
    gamepadReport.leftY = 127;  
  }
  if ((portVal & (1<<JOYSTICKLEFT)) == 0)
  {
    gamepadReport.leftX = -127;      
  }
  if ((portVal & (1<<JOYSTICKRIGHT)) == 0)
  {
    gamepadReport.leftX = 127;  
  }
 //   GPIO_PinOutToggle(ACTIVITY_LED);
 
      /* Send a key pushed report */
  if (0 != memcmp((void *)&lastGamepadReport,(void*) &gamepadReport,sizeof(gamepadReport_t)))
  { 
    USBD_Write(INTR_IN_EP_ADDR, (void*) &gamepadReport,
                   sizeof(gamepadReport_t), NULL);
    lastGamepadReport = gamepadReport;
    
    if (idleRate != 0)
    {
      USBTIMER_Start(IDLE_TIMER, idleRate, IdleTimeout);
    }
  }

  /* Restart HID poll timer */
    USBTIMER_Start(POLL_TIMER, DEFAULT_POLL_TIMEOUT, PollTimeout);
}
/**************************************************************************//**
 * @brief
 *   Called on timer elapsed event. This function is called at idle rate.
 *   if the idle rate is expired last sampled joystick and button state is sent.
 ******************************************************************************/
static void IdleTimeout(void)
{
  USBD_Write(INTR_IN_EP_ADDR, (void*) &gamepadReport,
                   sizeof(gamepadReport_t), NULL);
  USBTIMER_Start(IDLE_TIMER, idleRate, IdleTimeout);
}

/**************************************************************************//**
 * @brief
 *   Callback function called each time the USB device state is changed.
 *   Starts HID operation when device has been configured by USB host.
 *
 * @param[in] oldState The device state the device has just left.
 * @param[in] newState The new device state.
 *****************************************************************************/
static void StateChange(USBD_State_TypeDef oldState,
                        USBD_State_TypeDef newState)
{
  if (newState == USBD_STATE_CONFIGURED)
  {
    /* We have been configured, start HID functionality ! */
    if (oldState != USBD_STATE_SUSPENDED)   /* Resume ?   */
    {
      //keySeqNo    = 0;
    //  keyPushed   = false;
      idleRate    = DEFAULT_IDLE_RATE;
      GPIO_PinOutSet(ACTIVITY_LED);
    }
    USBTIMER_Start(POLL_TIMER, DEFAULT_POLL_TIMEOUT, PollTimeout);
    if (idleRate != 0)
    { 
       USBTIMER_Start(IDLE_TIMER, idleRate, IdleTimeout);
    }
  }

  else if ((oldState == USBD_STATE_CONFIGURED) &&
           (newState != USBD_STATE_SUSPENDED))
  {
    /* We have been de-configured, stop HID functionality */
    USBTIMER_Stop(POLL_TIMER);
    USBTIMER_Stop(IDLE_TIMER);    
    GPIO_PinOutClear(ACTIVITY_LED);
  }

  else if (newState == USBD_STATE_SUSPENDED)
  {
    /* We have been suspended, stop HID functionality */
    /* Reduce current consumption to below 2.5 mA.    */
    GPIO_PinOutClear(ACTIVITY_LED);
    USBTIMER_Stop(POLL_TIMER);
  }
}

/**************************************************************************//**
 * @brief
 *   Handle USB setup commands. Implements HID class specific commands.
 *
 * @param[in] setup Pointer to the setup packet received.
 *
 * @return USB_STATUS_OK if command accepted.
 *         USB_STATUS_REQ_UNHANDLED when command is unknown, the USB device
 *         stack will handle the request.
 *****************************************************************************/
static int SetupCmd(const USB_Setup_TypeDef *setup)
{
  STATIC_UBUF( hidDesc, USB_HID_DESCSIZE );

  int retVal = USB_STATUS_REQ_UNHANDLED;

  if ((setup->Type == USB_SETUP_TYPE_STANDARD) &&
      (setup->Direction == USB_SETUP_DIR_IN) &&
      (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE))
  {
    /* A HID device must extend the standard GET_DESCRIPTOR command   */
    /* with support for HID descriptors.                              */
    switch (setup->bRequest)
    {
    case GET_DESCRIPTOR:
      /********************/
      if ((setup->wValue >> 8) == USB_HID_REPORT_DESCRIPTOR)
      {
        USBD_Write(0, (void*) ReportDescriptor,
                   EFM32_MIN(sizeof(ReportDescriptor), setup->wLength),
                   NULL);
        retVal = USB_STATUS_OK;
      }
      else if ((setup->wValue >> 8) == USB_HID_DESCRIPTOR)
      {
        /* The HID descriptor might be misaligned ! */
        memcpy( hidDesc,
                &configDesc[ USB_CONFIG_DESCSIZE + USB_INTERFACE_DESCSIZE ],
                USB_HID_DESCSIZE );
        USBD_Write(0, hidDesc, EFM32_MIN(USB_HID_DESCSIZE, setup->wLength),
                   NULL);
        retVal = USB_STATUS_OK;
      }
      break;
    }
  }

  else if ((setup->Type == USB_SETUP_TYPE_CLASS) &&
           (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE))
  {
    /* Implement the necessary HID class specific commands.           */
    switch (setup->bRequest)
    {
    case USB_HID_SET_REPORT:
      /********************/
      if (((setup->wValue >> 8) == 2) &&            /* Output report */
          ((setup->wValue & 0xFF) == 0) &&          /* Report ID     */
          (setup->wIndex == 0) &&                   /* Interface no. */
          (setup->wLength == 1) &&                  /* Report length */
          (setup->Direction != USB_SETUP_DIR_IN))
      {
        USBD_Read(0, (void*) &tmpBuffer, 1, OutputReportReceived);
        retVal = USB_STATUS_OK;
      }
      break;

    case USB_HID_GET_REPORT:
      /********************/
      if (((setup->wValue >> 8) == 1) &&            /* Input report  */
          ((setup->wValue & 0xFF) == 0) &&          /* Report ID     */
          (setup->wIndex == 0) &&                   /* Interface no. */
          (setup->wLength == 8) &&                  /* Report length */
          (setup->Direction == USB_SETUP_DIR_IN))
      {

        USBD_Write(0, (void*) &gamepadReport,
                   sizeof(gamepadReport_t), NULL);

        retVal = USB_STATUS_OK;
      }
      break;

    case USB_HID_SET_IDLE:
      /********************/
      if (((setup->wValue & 0xFF) == 0) &&          /* Report ID     */
          (setup->wIndex == 0) &&                   /* Interface no. */
          (setup->wLength == 0) &&
          (setup->Direction != USB_SETUP_DIR_IN))
      {
        idleRate = setup->wValue >> 8;
        idleRate = idleRate << 2;
        if (idleRate < DEFAULT_POLL_TIMEOUT)
        {
          idleRate = DEFAULT_POLL_TIMEOUT;
        }
        /*Start og restart idle timer if idle rate != 0 */
        if (idleRate != 0)
        { 
          USBTIMER_Start(IDLE_TIMER, idleRate, IdleTimeout);
        }
        retVal = USB_STATUS_OK;
      }
      break;

    case USB_HID_GET_IDLE:
      /********************/
      if ((setup->wValue == 0) &&                   /* Report ID     */
          (setup->wIndex == 0) &&                   /* Interface no. */
          (setup->wLength == 1) &&
          (setup->Direction == USB_SETUP_DIR_IN))
      {
        *(uint8_t*)&tmpBuffer = idleRate >> 2;
        USBD_Write(0, (void*) &tmpBuffer, 1, NULL);
        retVal = USB_STATUS_OK;
      }
      break;
    }
  }

  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Callback function called when the data stage of a USB_HID_SET_REPORT
 *   setup command has completed.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int OutputReportReceived(USB_Status_TypeDef status,
                                uint32_t xferred,
                                uint32_t remaining)
{
  (void) status;
  (void) xferred;
  (void) remaining;

  return USB_STATUS_OK;
}
