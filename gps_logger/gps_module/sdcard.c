/**************************************************************************//**
 * @file main.c
 * @brief GPS-logger - SD Card
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

#include "ff.h"
#include "microsd.h"
#include "diskio.h"
   
/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/
   
#define BUFFERSIZE      1024

/*******************************************************************************
 ******************************   GLOBALS   ************************************
 ******************************************************************************/  

FIL fsrc;		                /* File objects */
FATFS Fatfs;				/* File system specific */
FRESULT res;				/* FatFs function common result code */
UINT br, bw;				/* File read/write count */
DSTATUS resCard;			/* SDcard status */
bool isInitialized = false;
int8_t ramBufferRead[BUFFERSIZE];	/* Temporary buffer for read file */
int16_t filecounter;

char *filenames[7] =  {"gps.dat","gps2.dat","gps3.dat","gps4.dat","gps5.dat","gps6.dat","gps7.dat"};
int8_t fileNum = 0;

/***************************************************************************//**
 * @brief
 *   This function is required by the FAT file system in order to provide
 *   timestamps for created files. Since this example does not include a 
 *   reliable clock we hardcode a value here.
 *
 *   Refer to drivers/fatfs/doc/en/fattime.html for the format of this DWORD.
 * @return
 *    A DWORD containing the current time and date as a packed datastructure.
 ******************************************************************************/
DWORD get_fattime(void)
{
  return (28 << 25) | (2 << 21) | (1 << 16);
}

/**************************************************************************//**
 * @brief
 *   SDCARD_Initialization.
 * @details
 *   Initialize SD Card
 *****************************************************************************/
void SDCARD_Initialization()
{
  /* Enable SPI */
  USART1->CMD = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
  
  volatile int waiting;
  /* Initialize SD Card */
  while(1)
  {
    MICROSD_Init();                     /*Initialize MicroSD driver */
      USART1->CMD = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;

    resCard = disk_initialize(0);       /*Check micro-SD card status */
    
    switch(resCard)
    {
    case STA_NOINIT:                    /* Drive not initialized */
      break;
    case STA_NODISK:                    /* No medium in the drive */
      break;
    case STA_PROTECT:                   /* Write protected */
      break;
    default:
      break;
    }
   
    if (!resCard) break;                /* Drive initialized. */
 
    /* wait a moment */
    for(waiting = 0; waiting < 0xfff; waiting++){
      waiting++;
      waiting--;
    }
  }
}

/**************************************************************************//**
 * @brief
 *   SDCARD_Save.
 * @details
 *   Save data into SD CARD
 *****************************************************************************/
void SDCARD_Save(sGPS_Data* data)
{
  FILINFO file_info;
  
  if(isInitialized == false){  
      /* Initialize filesystem */
      if (f_mount(0, &Fatfs) != FR_OK)
      {
        /* Error.No micro-SD with FAT32 is present */
        while(1);
      }
      
      /* Open  the file for write */
     isInitialized = true;
  }
  
   /* Read a file size */
   f_stat(filenames[fileNum], &file_info);
          
    if(file_info.fsize == 0){
      file_info.fsize++;
    }
  
    if(file_info.fsize>60000)
        fileNum++;
    
   res = f_open(&fsrc, filenames[fileNum],  FA_WRITE); 
   if (res != FR_OK)
   {
     /*  If file does not exist create it*/ 
     res = f_open(&fsrc, filenames[fileNum], FA_CREATE_ALWAYS | FA_WRITE ); 
      if (res != FR_OK) 
     {
      /* Error. Cannot create the file */
      while(1);
    }
   }
   
  /*Set the file write pointer to first location */ 
  res = f_lseek(&fsrc, file_info.fsize-1);
   if (res != FR_OK) 
  {
    /* Error. Cannot set the file write pointer */
    while(1);
  }

  /*Write a buffer to file*/
   res = f_write(&fsrc, data->string, data->length, &bw);
   if ((res != FR_OK) || (data->length != bw)) 
  {
    /* Error. Cannot write the file */
    while(1);
  }
  
  /* Close the file */
  f_close(&fsrc);
  if (res != FR_OK) 
  {
    /* Error. Cannot close the file */
    while(1);
  }  
}
