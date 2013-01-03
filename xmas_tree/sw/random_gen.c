/**************************************************************************//**
 * @file random_gen.c
 * @brief random_gen.c
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
#include "random_gen.h"
#include "em_device.h"
#include "em_cmu.h"

static uint32_t data[4];
static uint32_t count = 0;


uint32_t genrand(void)
{
  int i;
  /* No more data, need to generate some */
  if (count == 0)
  {
    for (i=0; i < 4; i++)
    {
      AES->DATA = data[i];
    }
    while (AES->STATUS & AES_STATUS_RUNNING) ;
    for (i=0; i <4; i++)
    {
      data[i] = AES->DATA;
    }
    count = 4;
  }
  count--;
  return data[count];
}

void genrand_init(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
   CMU_ClockEnable(cmuClock_AES, true);
   
   AES->KEYHA = a;
   AES->KEYHA = b;
   AES->KEYHA = c;
   AES->KEYHA = d;
   
   AES->CTRL = AES_CTRL_KEYBUFEN | AES_CTRL_DATASTART;
   count = 0;
}