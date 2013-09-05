#include "em_device.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "si114x_defs.h"

volatile uint8_t newValue = 0; /* new PS value */
static int x, y;
static int16_t old_x, old_y;
static bool old_result = false;
#define NUM_IR_CHANNELS 5
#define SI1143_ADDR 0x5A

#define printf(...) 

#define MIN_PS1 0x200
#define MIN_PS2 0x200
#define MIN_PS3 0x200
#define MAX_PS3 0xA00
#define PS1_PRESSED 0xC00
#define PS2_PRESSED PS1_PRESSED
#define PS3_PRESSED PS1_PRESSED

/**************************************************************************//**
 * @brief Initializes I2C peripheral
 *****************************************************************************/
void InitI2C(void)
{
  I2C_Init_TypeDef i2c_init = I2C_INIT_DEFAULT;

  /* Enable GPIO clock */
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
  
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* Pin PD14 is configured to Open-drain with pull-up and filter */
  GPIO->P[3].MODEH = (GPIO->P[3].MODEH & ~_GPIO_P_MODEH_MODE14_MASK) | GPIO_P_MODEH_MODE14_WIREDANDPULLUPFILTER;
  /* Pin PD15 is configured to Open-drain with pull-up and filter */
  GPIO->P[3].MODEH = (GPIO->P[3].MODEH & ~_GPIO_P_MODEH_MODE15_MASK) | GPIO_P_MODEH_MODE15_WIREDANDPULLUPFILTER;
  /* Pin PE1 is configured to Input enabled with pull-up and filter */
  GPIO->P[4].DOUT |= (1 << 1);
  GPIO->P[4].MODEL = (GPIO->P[4].MODEL & ~_GPIO_P_MODEL_MODE1_MASK) | GPIO_P_MODEL_MODE1_INPUTPULLFILTER;
  
  /* Enable clock for I2C0 */
  CMU_ClockEnable(cmuClock_I2C0, true);
  /* Module I2C0 is configured to location 3 */
  I2C0->ROUTE = (I2C0->ROUTE & ~_I2C_ROUTE_LOCATION_MASK) | I2C_ROUTE_LOCATION_LOC3;
  /* Enable signals SDA, SCL */
  I2C0->ROUTE |= I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN;  

  /* set interrupt on falling edge for port E1 */
  GPIO_IntConfig(gpioPortE, 1, false, true, true);
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  I2C_Init(I2C0, &i2c_init);
}

static int Si1143_ReadMultipleRegisters(uint8_t *data, uint8_t size, uint8_t reg)
{
  I2C_TransferSeq_TypeDef transfer;
  uint8_t i;
  I2C_TransferReturn_TypeDef ret;

  transfer.addr = SI1143_ADDR<<1;
  transfer.flags = I2C_FLAG_WRITE_READ;
  transfer.buf[0].len = 1;
  transfer.buf[0].data = &reg;
  transfer.buf[1].len = size;
  transfer.buf[1].data = data;

   // Read all the new data for each of the 5 measurements at once
   // There is a measurement per channel
  ret = I2C_TransferInit(I2C0, &transfer);
  while (ret == i2cTransferInProgress)
  {
     ret = I2C_Transfer(I2C0);
  }

  i = 0;
  
  if(ret == i2cTransferDone)
    i = transfer.buf[1].len;
  
  return(i);
}

static void Si1143_WriteRegister(uint8_t reg, uint8_t value)
{
  I2C_TransferSeq_TypeDef transfer;
  uint8_t dataBuf[2];
  I2C_TransferReturn_TypeDef ret;

  transfer.addr = SI1143_ADDR<<1;
  dataBuf[0] = reg;
  dataBuf[1] = value;
  transfer.flags = I2C_FLAG_WRITE;
  transfer.buf[0].len = 2;
  transfer.buf[0].data = dataBuf;
  transfer.buf[1].len = 1;
  transfer.buf[1].data = dataBuf;

  ret = I2C_TransferInit(I2C0, &transfer);
  while (ret == i2cTransferInProgress)
  {
     ret = I2C_Transfer(I2C0);
  }
}

#if 0
static void Si1143_WriteMultipleRegisters(uint8_t *data, uint8_t size, uint8_t reg)
{
  I2C_TransferSeq_TypeDef transfer;
  I2C_TransferReturn_TypeDef ret;

  transfer.addr = SI1143_ADDR<<1;
  transfer.flags = I2C_FLAG_WRITE;
  transfer.buf[0].len = 1;
  transfer.buf[0].data = &reg;
  transfer.buf[1].len = size;
  transfer.buf[1].data = data;

  ret = I2C_TransferInit(I2C0, &transfer);
  while (ret == i2cTransferInProgress)
  {
     ret = I2C_Transfer(I2C0);
  }
}

static uint8_t Si1143_ReadRegister(uint8_t reg)
{
  I2C_TransferSeq_TypeDef transfer;
  uint8_t regAddr;
  uint8_t dataBuf[2];
  I2C_TransferReturn_TypeDef ret;

  transfer.addr = SI1143_ADDR<<1;
  regAddr = reg;
  transfer.flags = I2C_FLAG_WRITE_READ;
  transfer.buf[0].len = 1;
  transfer.buf[0].data = &regAddr;
  transfer.buf[1].len = 1;
  transfer.buf[1].data = dataBuf;

  ret = I2C_TransferInit(I2C0, &transfer);
  while (ret == i2cTransferInProgress)
  {
     ret = I2C_Transfer(I2C0);
  }
   
   return dataBuf[0];
}
#endif

void PrintRegisters(void)
{
  uint8_t i, numberOfReadData;
  uint8_t dataBuf[0x40];

  numberOfReadData = Si1143_ReadMultipleRegisters(dataBuf, 0x3F, 0);

  for(i=0;i<numberOfReadData;i++)
  {
    if( (i % 8) == 0 )
      printf("\n%02X:", i);
    printf("%02X ", dataBuf[i]);
  }
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler (PE1)
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  /* Acknowledge interrupt */
  GPIO_IntClear(1 << 1);
  
  newValue = 1;
}

bool Si1143_ReadPos(int *ret_x, int *ret_y)
{
  uint8_t dataBuf[16];
  uint16_t ps1, ps2, ps3;
  bool result = false;
  
  if(newValue)
  {
    newValue = 0;

    /* read IR ADC values */
    Si1143_ReadMultipleRegisters(dataBuf, 6, 0x26);
    
    /* clear interrupt source */
    Si1143_WriteRegister(REG_IRQ_STATUS, 0x3F );

    ps1 = dataBuf[1]<<8 | dataBuf[0];
    ps2 = dataBuf[3]<<8 | dataBuf[2];
    ps3 = dataBuf[5]<<8 | dataBuf[4];
    x = 0;
    if(ps1>MIN_PS1) x = ps1 - MIN_PS1;
    if(ps2>MIN_PS2) x += ps2 - MIN_PS2;
    y = x/4;
    if(ps3>MIN_PS3)
      y += ps3 - MIN_PS3;
    if(x!=0)
    {
      if(ps1>MIN_PS1)
        x = 320 - (320*(ps1-MIN_PS1))/x;  
      else
        x = -1;
    } else x = -1;
    if(y!=0)
    {
      if(ps3>MIN_PS3)
        y = 240 - (240*(ps3-MIN_PS3))/(MAX_PS3-MIN_PS3);
      else
        y = -1;
    } else y = -1;
    if(y>240) 
      y = 240;

    /* Output text on TFT - using address mapped mode */
    printf("PS1 %04X PS2 %04X PS3 %04X x=%d y=%d  \r", ps1, ps2, ps3, x, y);
    if( (ps1>PS1_PRESSED) || (ps2>PS2_PRESSED) || (ps3>PS3_PRESSED))
      result = true;
  };
  
  if(result || old_result)
  {
    *ret_x = old_x;
    *ret_y = old_y;
  }
  else
  {
    *ret_x = x;
    *ret_y = y;
    old_x = x;
    old_y = y;
  }
  old_result = result;
  
  return(result);
}

bool Si1143_NewEvent(void)
{
  return(newValue != 0);
}

void Si1143_SetParam(uint8_t addr, uint8_t value)
{
   Si1143_WriteRegister(REG_PARAM_WR, value);
   Si1143_WriteRegister(REG_COMMAND, 0xA0 | addr);

}
void Si1143_Init(void)
{
  InitI2C();

  Si1143_WriteRegister(REG_COMMAND, 0x0B);
  /* Reset device */
  Si1143_WriteRegister(REG_COMMAND, 0x01);

  Si1143_WriteRegister(REG_IRQ_CFG, 0);            // Tri-state INT pin   
  Si1143_WriteRegister(REG_IRQ_ENABLE, IE_NONE);
  Si1143_WriteRegister(REG_IRQ_MODE1, 0x00);
  Si1143_WriteRegister(REG_IRQ_MODE2, 0x00);

  // Send Hardware Key
  Si1143_WriteRegister(REG_HW_KEY, HW_KEY_VAL0);

  // Set LED currents to max
  Si1143_WriteRegister(REG_PS_LED21, 0xFF); 
  Si1143_WriteRegister(REG_PS_LED3,  0x0F); 

  /* scan PS1, PS2, PS3 */
  Si1143_SetParam(PARAM_CH_LIST, 0x07);
  Si1143_SetParam(PARAM_PS_ADC_COUNTER,RECCNT_007);
  Si1143_SetParam(PARAM_ALSVIS_ADC_COUNTER,RECCNT_001);
  Si1143_SetParam(PARAM_ALSIR_ADC_COUNTER,RECCNT_001);

  // Set the Hardware key
  Si1143_WriteRegister(REG_HW_KEY, HW_KEY_VAL0);
      
  // Enable INT pin
  Si1143_WriteRegister(REG_IRQ_ENABLE, IE_PS3_EVRYSAMPLE);
  Si1143_WriteRegister(REG_IRQ_CFG, ICG_INTOE);  
  Si1143_WriteRegister(REG_IRQ_ENABLE, IE_PS3_EVRYSAMPLE );  
  Si1143_WriteRegister(REG_IRQ_MODE1, 
        IM1_ALS_EVRYSAMPLE + IM1_PS1_EVRYSAMPLE + IM1_PS2_EVRYSAMPLE );
  Si1143_WriteRegister(REG_IRQ_MODE2, IM2_PS3_EVRYSAMPLE);
   
  /* set measurement rate for 10Hz */
  Si1143_WriteRegister(REG_MEAS_RATE, 0x80);
  Si1143_WriteRegister(REG_PS_RATE,   0x08);
  Si1143_WriteRegister(REG_ALS_RATE,  0x08);

  Si1143_WriteRegister(REG_COMMAND,0x0F);
}
