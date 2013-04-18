/**************************************************************************//**
 * @file tftdraw.c
 * @brief tftdraw.c
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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "efm32.h"
#include "bsp.h"

#include "glib/glib.h"
#include "glib/glib_color.h"
#include "glib/glib_font.h"
#include "dmd/ssd2119/dmd_ssd2119.h"

#include "tftdraw.h"

#define NUMBER_OF_KASSES 1
#define BEERS_IN_KASSE 12
#define BEER_SIZE 30 //Size of beer in pixels. Should be divisible with 2.
#define BEER_DISPLACEMENT 30 //Distance from Edge of case
#define MAX_VOLT 0xFFF
#define MAX_TEMP 12//Max temp when beer is hot¨
#define ROOM_TEMP 20 //Room temperature

extern void Delay(uint32_t dlyTicks);

/**************************************************************************//**
 * @brief Clears/updates entire background ready to be drawn
 *****************************************************************************/
void TFT_displayUpdate(GLIB_Context *gcPtr, uint32_t samples[], uint8_t numSamples)
{
  uint16_t temp;
  
  GLIB_Rectangle rect = {
    .xMin =   0,
    .yMin =   0,
    .xMax = 319,
    .yMax = 239,
  };
  static char    *efm32_hello = "     12 bottles - one giant\n";

  /* Set clipping region to entire image */
  GLIB_setClippingRegion(gcPtr, &rect);
  GLIB_resetDisplayClippingArea(gcPtr);
  
  //Redraw backgound
  gcPtr->foregroundColor = GLIB_rgbColor(20, 40, 20);
//  GLIB_drawRectFilled(gcPtr, &rect);

  /* Update text on top of picture  */
  gcPtr->foregroundColor = GLIB_rgbColor(200, 200, 200);
  gcPtr->backgroundColor = GLIB_rgbColor(0,0,0);
  GLIB_drawString(gcPtr, efm32_hello, strlen(efm32_hello), 0, 0, 1);
  
  uint32_t drawColor = 0;
  char tmp[5] = "\0";
  //Populate kasse
  for (int j=0; j <NUMBER_OF_KASSES; j++) {
    uint16_t xDis = BEER_DISPLACEMENT + (BEER_DISPLACEMENT + (4*BEER_SIZE))*j;
    uint16_t yDis = BEER_DISPLACEMENT;
    //Draw kasse
    gcPtr->foregroundColor = GLIB_rgbColor(0, 100, 0);
    GLIB_Rectangle kasse = {
      xDis,
      yDis,
      xDis + (4*BEER_SIZE),
      yDis + (6*BEER_SIZE),
    };

    GLIB_drawRectFilled(gcPtr, &kasse);
    
    
    xDis += (3*BEER_SIZE) + (BEER_SIZE / 2);
    yDis += (5*BEER_SIZE) + (BEER_SIZE / 2);
    
    for (int i=0; i < BEERS_IN_KASSE; i++) {
       
      //Get voltage
      if(i >= numSamples) {
        //Grey if we don't measure it
        drawColor = GLIB_rgbColor(100, 100, 100);
        temp = 42;
      }
      else {
        temp = samples[i] / 33 + 2;
        if (temp > ROOM_TEMP) {
          //Grey when no beer
          drawColor = GLIB_rgbColor(100, 100, 100);
        }
        else if (temp > MAX_TEMP) {
          drawColor = GLIB_rgbColor(255 , 0, ((ROOM_TEMP-temp)*255)/(ROOM_TEMP - MAX_TEMP) );
        }
        else {
          drawColor = GLIB_rgbColor(((MAX_TEMP-temp-2)*255)/(MAX_TEMP - 2), 0, 255);
        }
      }
      
      gcPtr->foregroundColor = drawColor;
      //This is correct
      GLIB_drawCircleFilled(gcPtr, xDis - (i % 4)*BEER_SIZE, yDis - (i / 4)*BEER_SIZE, (BEER_SIZE / 2) - 2);
      
      //Write temp. This only print the two first numbers of the temp
      sprintf(tmp, "%.2d", temp);
      gcPtr->backgroundColor = drawColor;
      gcPtr->foregroundColor = GLIB_rgbColor(255,255,255);
      GLIB_drawString(gcPtr, tmp, strlen(tmp), xDis - (i % 4)*BEER_SIZE - (BEER_SIZE / 4), yDis - (i / 4)*BEER_SIZE- (BEER_SIZE / 4), 1);
    }
  
  }
  
}
