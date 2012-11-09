/**************************************************************************//**
 * @file
 * @brief Ikea DIODER PWM driver
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
#include "stdlib.h"
/* EM header files */
#include "em_part.h"

/* Drivers */
#include "em_timer.h"
#include "em_cmu.h"
#include "em_gpio.h"



/* Capacitive sense configuration */
#include "ikea_pwm.h"

/**************************************************************************//**
 * Macro definitions
 *****************************************************************************/


/**************************************************************************//**
 * Prototypes
 *****************************************************************************/


/**************************************************************************//**
 * Local variables
 *****************************************************************************/

/**************************************************************************//**
 * @brief  Init
 *****************************************************************************/
void IKEADIODER_Init(void)
{
#warning "TODO remember to change USB TIMER to timer 1"  
  /* Enable clock for GPIO module */
  CMU_ClockEnable(cmuClock_GPIO, true);
  
  /* Enable clock for TIMER0 module */
  CMU_ClockEnable(IKEA_TIMER_CMU, true);

#if (GG_STK_SETTINGS == true)
  /* Set CC0 location 3 pin (PD1) as output */
  GPIO_PinModeSet(gpioPortD, 1, gpioModePushPull, 0);
  /* Set CC1 location 2 pin (PD2) as output */
  GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0);
  /* Set CC2 location 3 pin (PD3) as output */
  GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 0);  

  /* Route CC0 to location 3 (PD1) and enable pin */  
  IKEA_TIMER->ROUTE |= (TIMER_ROUTE_CC0PEN | TIMER_ROUTE_CC1PEN |TIMER_ROUTE_CC2PEN | TIMER_ROUTE_LOCATION_LOC3); 

#else
  
  /* Set CC0 location 4 pin (PA0) as output */
  GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 0);
  /* Set CC1 location 4 pin (PC0) as output */
  GPIO_PinModeSet(gpioPortC, 0, gpioModePushPull, 0);
  /* Set CC2 location 4 pin (PC1) as output */
  GPIO_PinModeSet(gpioPortE, 1, gpioModePushPull, 0);  

  /* Route CC0 to location 3 (PD1) and enable pin */  
  IKEA_TIMER->ROUTE |= (TIMER_ROUTE_CC0PEN | TIMER_ROUTE_CC1PEN |TIMER_ROUTE_CC2PEN | TIMER_ROUTE_LOCATION_LOC4); 

#endif
  /* Select CC channel parameters */
  TIMER_InitCC_TypeDef timerCCInit = 
  {
    .eventCtrl  = timerEventEveryEdge,
    .edge       = timerEdgeBoth,
    .prsSel     = timerPRSSELCh0,
    .cufoa      = timerOutputActionNone,
    .cofoa      = timerOutputActionNone,
    .cmoa       = timerOutputActionToggle,
    .mode       = timerCCModePWM,
    .filter     = false,
    .prsInput   = false,
    .coist      = false,
    .outInvert  = false,
  };
  
  /* Configure CC channel 0 */
  TIMER_InitCC(IKEA_TIMER, 0, &timerCCInit);
  /* Configure CC channel 1 */
  TIMER_InitCC(IKEA_TIMER, 1, &timerCCInit);
  /* Configure CC channel 2 */
  TIMER_InitCC(IKEA_TIMER, 2, &timerCCInit);

    
  /* Set Top Value */
  TIMER_TopSet(IKEA_TIMER, IKEA_TIMER_TOP);
  
  TIMER_CompareBufSet(IKEA_TIMER, 0, 0x7F);
  TIMER_CompareBufSet(IKEA_TIMER, 1, 0x7F);
  TIMER_CompareBufSet(IKEA_TIMER, 2, 0x7F);

  /* Select timer parameters */  
  
  /* With a clock of 48MHZ, a divider of 512 and a resolution of
     256/8 bit the PWM period frequency is approx  366 hz */
  
  TIMER_Init_TypeDef timerInit =
  {
    .enable     = true,
    .debugRun   = true,
    .prescale   = timerPrescale512,
    .clkSel     = timerClkSelHFPerClk,
    .fallAction = timerInputActionNone,
    .riseAction = timerInputActionNone,
    .mode       = timerModeUp,
    .dmaClrAct  = false,
    .quadModeX4 = false,
    .oneShot    = false,
    .sync       = false,
  };
  
  /* Configure timer */
  TIMER_Init(IKEA_TIMER, &timerInit);  
}


void IKEADIODER_RGBColorSet(uint8_t red, uint8_t green, uint8_t blue)
{
  TIMER_CompareBufSet(IKEA_TIMER, 0, red);
  TIMER_CompareBufSet(IKEA_TIMER, 1, green);
  TIMER_CompareBufSet(IKEA_TIMER, 2, blue);
}

// http://www.ah-andersen.net/index.php?site=hsvtorgb&menu=programming
void IKEADIODER_HSVColorSet(uint8_t h, uint8_t s, uint8_t v)
{
   uint8_t hb;
   uint8_t r,g,b;
   unsigned int chroma, x;
 
   h *= 3;			//Changes the range of h from 0-255 to 0-765 (which is 
   				//almost 0-767).
      
   chroma = s * v;		//Chroma ends up in the range 0-(255*255) => 0-65025.
   chroma = chroma>>8;		//Using binary shift do divide with 256. 
   				//The range is now approximately 0-255.
       
   hb = h;			//The value of h is now assigned to a byte. This is 
   				//equivalent to taking modulo 256 of h, as the byte will overflow.
    
   
   x = chroma * (128 - abs( hb - 128) );	//x is calculated. 1 is replaced by 128 and 
   						//mod2 is replaced by the previous mod256 on h.	
   
   x = x>>7; 			//Dividing with 128 to fit x in the interval 0-255.
   				//This has to be done because of the changes we made to 
   				//the calculation of x.
  
   h = h>>7;			//h(0-767) is now divided with 128 so that becomes a number 0-5. 
 
   switch( h ) 			//Calculates the result, depending on which number from 
   				//0-5 h has become, and stores it in r, g and b.
   {
      case 0:
         r = v;
         g = x + v - chroma;
         b = v-chroma;
         break;
      case 1:
         g = v;
         r = x + v - chroma;
         b = v-chroma;
         break;
      case 2:
         g = v;
         b = x + v - chroma;
         r = v-chroma;
         break;
      case 3:
         b = v;
         g = x + v - chroma;
         r = v-chroma;
         break;
      case 4:
         b = v;
         r = x + v - chroma;
         g = v-chroma;
         break;
      case 5:
         r = v;
         b = x + v - chroma;
         g = v-chroma;
         break;  
   } 
  IKEADIODER_RGBColorSet(r,g,b); 
}
