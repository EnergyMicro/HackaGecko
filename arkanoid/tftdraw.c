/**************************************************************************//**
 * @file
 * @brief EFM32WG_DK3850 TFT address mapped example, drawing random rectangles
 * @author Energy Micro AS
 * @version 3.20.0
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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "em_device.h"
#include "bsp.h"

#include "glib/glib.h"
#include "glib/glib_color.h"
#include "glib/glib_font.h"
#include "dmd/ssd2119/dmd_ssd2119.h"

#include "tftdraw.h"
#include "si1143.h"

#define BALL_R 5
#define SHIP_X (320/2)
#define SHIP_Y 225
#define SHIP_SIZE 20
#define BRICK_X_SIZE 16
#define BRICK_Y_SIZE 8
#define BRICKS_MAX 100
#define HIT_NO 0
#define HIT_X 1
#define HIT_Y 2

typedef struct {
  uint16_t x;
  uint16_t y;
  uint32_t color;
  bool status;
} Brick_t;

Brick_t bricks[BRICKS_MAX];
static bool initRequired = true;

int ship_x = SHIP_X, ship_y = SHIP_Y;
uint8_t ship_size = SHIP_SIZE;
uint16_t ball_x = SHIP_X, ball_y = SHIP_Y - BALL_R, ball_r = BALL_R;
int move_x = 0, move_y = 0;

void tft_ShipRectangle(GLIB_Rectangle *rect, int x)
{
  rect->xMin = x>ship_size?x-ship_size:0;
  rect->xMax = x<319-ship_size?x+ship_size:319;
  rect->yMin = ship_y;
  rect->yMax = ship_y+10;
}

void tft_DrawBrick(GLIB_Context *gcPtr, Brick_t *brick)
{ GLIB_Rectangle rect;

  gcPtr->foregroundColor = brick->color;
  rect.xMin = brick->x - (BRICK_X_SIZE-2);
  rect.xMax = brick->x + (BRICK_X_SIZE-2);
  rect.yMin = brick->y - (BRICK_Y_SIZE-2);
  rect.yMax = brick->y + (BRICK_Y_SIZE-2);
  
  GLIB_drawRectFilled(gcPtr, &rect);
}

void tft_InitGame(GLIB_Context *gcPtr)
{ 
  uint16_t x, y, brick=0;
  
  for(y = 30; y < 150; y += 2*BRICK_Y_SIZE)
     for(x = BRICK_X_SIZE + 20; x < (319 - BRICK_X_SIZE); x += 2*BRICK_X_SIZE)
     {
       bricks[brick].x = x;
       bricks[brick].y = y;
       bricks[brick].status = true;
       bricks[brick].color = GLIB_rgbColor(y, 255-y, 255);
       tft_DrawBrick(gcPtr, &bricks[brick]);
       brick++;
     }
     while(brick < BRICKS_MAX)
       bricks[brick++].status = false;
  
    move_x = 0;
    move_y = 0;
    initRequired = false;
}

int8_t tft_CheckBricksCollision(int16_t x, int16_t y)
{ int16_t min_x, max_x, min_y, max_y;
  int8_t i = 0;
  int8_t hit = -1;
  
  min_x = x - BRICK_X_SIZE;
  max_x = x + BRICK_X_SIZE;
  min_y = y - ball_r - BRICK_Y_SIZE;
  max_y = y + ball_r + BRICK_Y_SIZE;
  
  if(min_x<0) min_x = 0;
  if(max_x>319) max_x = 319;
  if(min_y<0) min_y = 0;
  if(max_y>239) max_y = 239;
  
  while( (i<BRICKS_MAX) && (hit<0) )
  {
    if( bricks[i].status )
      if( (bricks[i].x >= min_x) && (bricks[i].x <= max_x) )
        if( (bricks[i].y >= min_y) && (bricks[i].y <= max_y) )
          hit = i;
    if(hit<0)
      i++;
  }
  
  return(hit);
}

void tft_RemoveBrick(GLIB_Context *gcPtr, uint8_t i)
{
  bricks[i].status = false;
  bricks[i].color =  GLIB_rgbColor(20, 40, 20);
  tft_DrawBrick(gcPtr, &bricks[i]);
}

bool tft_MoveBall(GLIB_Context *gcPtr)
{ int16_t x, y;
  int16_t ship_hit, new_speed;
  int8_t hit;
  
  x = ball_x + move_x;
  y = ball_y + move_y;
  if(x < ball_r)
  { x = ball_r;
    move_x = -move_x;
  }
  if(x > (319 - ball_r) )
  {
    x = 319 - ball_r;
    move_x = -move_x;
  }
  if(y < ball_r)
  {
    y = ball_r;
    move_y = -move_y;
  }
  /* check if we hit into ship */
  if( y  >= (ship_y - ball_r) )
  {
    if( (x > ship_x - ship_size) && (x < ship_x + ship_size) )
    { /* ball hit ship */
      if(move_y > 0)
      {
        move_y = -move_y;
        ship_hit = abs(ship_x - x);
        if(ship_hit < ship_size/2)
          new_speed = 1;
        else if(ship_hit > ship_size*3/4)
          new_speed = 2;
        else new_speed = 3;
        move_x = move_x>0?new_speed:-new_speed; 
      }
      y = ship_y - ball_r;
    }
  } else
  {
    /* returns number of hit brick or -1 if none */
    hit = tft_CheckBricksCollision(x, y);
    if(hit>=0)
    { int16_t dist_x = abs(bricks[hit].x - x);
      int16_t dist_y = abs(bricks[hit].y - y);

      if(dist_x == BRICK_X_SIZE)
        move_x = -move_x;
      if(dist_y == BRICK_Y_SIZE+ball_r)
        move_y = -move_y;

      tft_RemoveBrick(gcPtr, hit);
    }
  }

  if(y > (239 - ball_r) )
    initRequired = true;
  
  if( (ball_x != x) || (ball_y != y) )
  {
    gcPtr->foregroundColor = GLIB_rgbColor(20, 40, 20);
    GLIB_drawCircleFilled(gcPtr, ball_x, ball_y, ball_r);
    
    gcPtr->foregroundColor = GLIB_rgbColor(255,255,255);
    GLIB_drawCircleFilled(gcPtr, x, y, ball_r);
    
    ball_x = x;
    ball_y = y;
  }
  
  return(initRequired);
}

/**************************************************************************//**
 * @brief Clears/updates entire background ready to be drawn
 *****************************************************************************/
void TFT_displayUpdate(GLIB_Context *gcPtr)
{
  GLIB_Rectangle rect = {
    .xMin =   0,
    .yMin =   0,
    .xMax = 319,
    .yMax = 239,
  };
 // static char    *efm32_hello = "48MHz / SSD2119 address mapped TFT\n";

  /* Set clipping region to entire image */
  GLIB_setClippingRegion(gcPtr, &rect);
  GLIB_resetDisplayClippingArea(gcPtr);

  /* Update text on top of picture  */
  //gcPtr->foregroundColor = GLIB_rgbColor(200, 200, 200);
  //GLIB_drawString(gcPtr, efm32_hello, strlen(efm32_hello), 0, 0, 1);

  if(initRequired)
    tft_InitGame(gcPtr);
  
  if(Si1143_NewEvent())
  { int x, y;
    
    Si1143_ReadPos(&x, &y);
    if(x<0)
      return; /* hand taken away - pause */

    if(x < ship_size)
      x = ship_size;
    if(x > 319 - ship_size)
      x = 319 - ship_size;
    if(x != ship_x)
    {
      tft_ShipRectangle(&rect, ship_x);
      gcPtr->foregroundColor = GLIB_rgbColor(20, 40, 20);
      GLIB_drawRectFilled(gcPtr, &rect);
      
      tft_ShipRectangle(&rect, x);
      gcPtr->foregroundColor = GLIB_rgbColor(255,255,0);
      GLIB_drawRectFilled(gcPtr, &rect);
      ship_x = x;
      
    }
    if(move_y == 0)
    {
      if( (y>0) && (y > 50) )
      {
        move_y = -1;
        move_x = 0;
      }
      else
        move_x = ship_x - ball_x;
    }
    tft_MoveBall(gcPtr);
  }
}
