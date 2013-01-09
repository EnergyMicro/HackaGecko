/***************************************************************************//**
 * @file descriptors.h
 * @brief USB descriptors for HID keyboard example project.
 * @author Energy Micro AS
 * @version 1.0.2
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

/* Created with usb.org's "HID Descriptor Tool(DT)", version 2.4 */

#ifdef __cplusplus
extern "C" {
#endif

EFM32_ALIGN(4)
static const char ReportDescriptor[42] __attribute__ ((aligned(4)))=
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x06,                    //     USAGE_MAXIMUM (Button 6)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x10,                    //     REPORT_COUNT (16)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
//    0x09, 0x32,                    //     USAGE (Z)
 //   0x09, 0x33,                    //     USAGE (Rx)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};



EFM32_ALIGN(4)
static const USB_DeviceDescriptor_TypeDef deviceDesc __attribute__ ((aligned(4)))=
{
  .bLength            = USB_DEVICE_DESCSIZE,
  .bDescriptorType    = USB_DEVICE_DESCRIPTOR,
  .bcdUSB             = 0x0200,
  .bDeviceClass       = 0,
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = USB_EP0_SIZE,
  .idVendor           = 0x2544,
  .idProduct          = 0x0002,
  .bcdDevice          = 0x0000,
  .iManufacturer      = 1,
  .iProduct           = 2,
  .iSerialNumber      = 3,
  .bNumConfigurations = 1
};

EFM32_ALIGN(4)
static const uint8_t configDesc[] __attribute__ ((aligned(4)))=
{
  /*** Configuration descriptor ***/
  USB_CONFIG_DESCSIZE,    /* bLength                                   */
  USB_CONFIG_DESCRIPTOR,  /* bDescriptorType                           */

  USB_CONFIG_DESCSIZE +   /* wTotalLength (LSB)                        */
  USB_INTERFACE_DESCSIZE +
  USB_HID_DESCSIZE +
  (USB_ENDPOINT_DESCSIZE * NUM_EP_USED),

  (USB_CONFIG_DESCSIZE +  /* wTotalLength (MSB)                        */
  USB_INTERFACE_DESCSIZE +
  USB_HID_DESCSIZE +
  (USB_ENDPOINT_DESCSIZE * NUM_EP_USED))>>8,

  1,                      /* bNumInterfaces                            */
  1,                      /* bConfigurationValue                       */
  0,                      /* iConfiguration                            */

#if defined(BUSPOWERED)
  CONFIG_DESC_BM_RESERVED_D7,    /* bmAttrib: Bus powered              */
#else
  CONFIG_DESC_BM_RESERVED_D7 |   /* bmAttrib: Self powered             */
  CONFIG_DESC_BM_SELFPOWERED,
#endif

  CONFIG_DESC_MAXPOWER_mA( 50 ), /* bMaxPower: 50 mA                   */

  /*** Interface descriptor ***/
  USB_INTERFACE_DESCSIZE, /* bLength               */
  USB_INTERFACE_DESCRIPTOR,/* bDescriptorType      */
  0,                      /* bInterfaceNumber      */
  0,                      /* bAlternateSetting     */
  NUM_EP_USED,            /* bNumEndpoints         */
  0x03,                   /* bInterfaceClass (HID) */
  0,                      /* bInterfaceSubClass    */
  1,                      /* bInterfaceProtocol    */
  0,                      /* iInterface            */

  /*** HID descriptor ***/
  USB_HID_DESCSIZE,       /* bLength               */
  USB_HID_DESCRIPTOR,     /* bDescriptorType       */
  0x11,                   /* bcdHID (LSB)          */
  0x01,                   /* bcdHID (MSB)          */
  0,                      /* bCountryCode          */
  1,                      /* bNumDescriptors       */
  USB_HID_REPORT_DESCRIPTOR,     /* bDecriptorType        */
  sizeof( ReportDescriptor ),    /* wDescriptorLength(LSB)*/
  sizeof( ReportDescriptor )>>8, /* wDescriptorLength(MSB)*/

  /*** Endpoint descriptor ***/
  USB_ENDPOINT_DESCSIZE,  /* bLength               */
  USB_ENDPOINT_DESCRIPTOR,/* bDescriptorType       */
  INTR_IN_EP_ADDR,        /* bEndpointAddress (IN) */
  USB_EPTYPE_INTR,        /* bmAttributes          */
  USB_MAX_EP_SIZE,        /* wMaxPacketSize (LSB)  */
  0,                      /* wMaxPacketSize (MSB)  */
  DEFAULT_POLL_TIMEOUT,   /* bInterval             */
};

STATIC_CONST_STRING_DESC_LANGID( langID, 0x04, 0x09         );
STATIC_CONST_STRING_DESC( iManufacturer, L"Energy Micro AS" );
STATIC_CONST_STRING_DESC( iProduct     , L"EFM32 USB HID Gamepad" );
STATIC_CONST_STRING_DESC( iSerialNumber, L"000000001234"    );

static const void * const strings[] =
{
  &langID,
  &iManufacturer,
  &iProduct,
  &iSerialNumber
};

/* Endpoint buffer sizes */
/* 1 = single buffer, 2 = double buffering, 3 = triple buffering ... */
static const uint8_t bufferingMultiplier[ NUM_EP_USED + 1 ] = { 1, 1 };

static const USBD_Callbacks_TypeDef callbacks =
{
  .usbReset        = NULL,
  .usbStateChange  = StateChange,
  .setupCmd        = SetupCmd,
  .isSelfPowered   = NULL,
  .sofInt          = NULL
};

static const USBD_Init_TypeDef initstruct =
{
  .deviceDescriptor    = &deviceDesc,
  .configDescriptor    = configDesc,
  .stringDescriptors   = strings,
  .numberOfStrings     = sizeof(strings)/sizeof(void*),
  .callbacks           = &callbacks,
  .bufferingMultiplier = bufferingMultiplier,
  .reserved            = 0
};

/*gamepad report structure*/
typedef struct
{
	uint16_t buttons;
	int8_t leftX;
	int8_t leftY;
} gamepadReport_t;



#ifdef __cplusplus
}
#endif
