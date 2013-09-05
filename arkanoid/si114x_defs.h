//-----------------------------------------------------------------------------
// Si114x_defs.h
//-----------------------------------------------------------------------------
// Copyright 2011 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:  
//
// Register/bit definitions for the Si114x family.
//
// Target:         Si114x 
// Command Line:   None
//
// Engineering Version
// 
//-----------------------------------------------------------------------------

#ifndef SI114X_DEFS_H
#define SI114X_DEFS_H

// I2C Registers
#define REG_PART_ID               0x00
#define REG_REV_ID                0x01
#define REG_SEQ_ID                0x02
#define REG_IRQ_CFG               0x03
#define REG_INT_CFG               0x03
#define REG_IRQ_ENABLE            0x04
#define REG_IRQ_MODE1             0x05
#define REG_IRQ_MODE2             0x06
#define REG_HW_KEY                0x07
#define REG_MEAS_RATE             0x08
#define REG_ALS_RATE              0x09
#define REG_PS_RATE               0x0A
#define REG_ALS_LO_TH             0x0B
#define REG_ALS_HI_TH             0x0D
#define REG_ALS_IR_ADCMUX         0x0E
#define REG_PS_LED21              0x0F
#define REG_PS_LED3               0x10
#define REG_PS1_TH                0x11
#define REG_PS2_TH                0x13
#define REG_PS3_TH                0x15
#define REG_PARAM_IN              0x17
#define REG_PARAM_WR              0x17
#define REG_COMMAND               0x18
#define REG_RESPONSE              0x20
#define REG_IRQ_STATUS            0x21
#define REG_ALS_VIS_DATA0         0x22
#define REG_ALS_VIS_DATA1         0x23
#define REG_ALS_IR_DATA0          0x24
#define REG_ALS_IR_DATA1          0x25
#define REG_PS1_DATA0             0x26
#define REG_PS1_DATA1             0x27
#define REG_PS2_DATA0             0x28
#define REG_PS2_DATA1             0x29
#define REG_PS3_DATA0             0x2A
#define REG_PS3_DATA1             0x2B
#define REG_AUX_DATA0             0x2C
#define REG_AUX_DATA1             0x2D
#define REG_PARAM_OUT             0x2E
#define REG_PARAM_RD              0x2E
#define REG_CHIP_STAT             0x30


// Parameter Offsets
#define PARAM_I2C_ADDR            0x00
#define PARAM_CH_LIST             0x01
#define PARAM_PSLED12_SELECT      0x02
#define PARAM_PSLED3_SELECT       0x03
#define PARAM_FILTER_EN           0x04
#define PARAM_PS_ENCODING         0x05
#define PARAM_ALS_ENCODING        0x06
#define PARAM_PS1_ADC_MUX         0x07
#define PARAM_PS2_ADC_MUX         0x08
#define PARAM_PS3_ADC_MUX         0x09
#define PARAM_PS_ADC_COUNTER      0x0A
#define PARAM_PS_ADC_CLKDIV       0x0B
#define PARAM_PS_ADC_GAIN         0x0B
#define PARAM_PS_ADC_MISC         0x0C
#define PARAM_ALS1_ADC_MUX        0x0D
#define PARAM_ALS2_ADC_MUX        0x0E
#define PARAM_ALS3_ADC_MUX        0x0F
#define PARAM_ALSVIS_ADC_COUNTER  0x10
#define PARAM_ALSVIS_ADC_CLKDIV   0x11
#define PARAM_ALSVIS_ADC_GAIN     0x11
#define PARAM_ALSVIS_ADC_MISC     0x12
#define PARAM_ALS_HYST            0x16
#define PARAM_PS_HYST             0x17
#define PARAM_PS_HISTORY          0x18
#define PARAM_ALS_HISTORY         0x19
#define PARAM_ADC_OFFSET          0x1A
#define PARAM_SLEEP_CTRL          0x1B
#define PARAM_LED_RECOVERY        0x1C
#define PARAM_ALSIR_ADC_COUNTER   0x1D
#define PARAM_ALSIR_ADC_CLKDIV    0x1E
#define PARAM_ALSIR_ADC_GAIN      0x1E
#define PARAM_ALSIR_ADC_MISC      0x1F



// REG_IRQ_CFG 
#define  ICG_INTOE                0x01
#define  ICG_INTMODE              0x02


// REG_IRQ_ENABLE
// REG_IRQ_STATUS
#define IE_NONE                   0x00

#define IE_ALS_NONE               0x00
#define IE_ALS_EVRYSAMPLE         0x01
#define IE_ALS_EXIT_WIN           0x01
#define IE_ALS_ENTER_WIN          0x02

#define IE_PS1_NONE               0x00
#define IE_PS1_EVRYSAMPLE         0x04
#define IE_PS1_CROSS_TH           0x04
#define IE_PS1_EXCEED_TH          0x04
#define IE_PS1                    0x04

#define IE_PS2_NONE               0x00
#define IE_PS2_EVRYSAMPLE         0x08
#define IE_PS2_CROSS_TH           0x08
#define IE_PS2_EXCEEED_TH         0x08
#define IE_PS2                    0x08

#define IE_PS3_NONE               0x00
#define IE_PS3_EVRYSAMPLE         0x10
#define IE_PS3_CROSS_TH           0x10
#define IE_PS3_EXCEED_TH          0x10
#define IE_PS3                    0x10

#define IE_CMD                    0x20

#define IE_ALL                    0x3F

// REG_IRQ_MODE1
#define IM1_NONE                  0x00
#define IM1_ALS_NONE              0x00
#define IM1_ALS_EVRYSAMPLE        0x00
#define IM1_ALS_VIS_EXIT          0x01
#define IM1_ALS_VIS_ENTER         0x05
#define IM1_ALS_IR_EXIT           0x03
#define IM1_ALS_IR_ENTER          0x06

#define IM1_PS1_NONE              0x00
#define IM1_PS1_EVRYSAMPLE        (0x0<<4)
#define IM1_PS1_CROSS_TH          (0x1<<4)
#define IM1_PS1_EXCEED_TH         (0x3<<4)

#define IM1_PS2_NONE              0x00
#define IM1_PS2_EVRYSAMPLE        (0x0<<6)
#define IM1_PS2_CROSS_TH          (0x1<<6)
#define IM1_PS2_EXCEED_TH         (0x3<<6)


// REG_IRQ_MODE1
#define IM2_PS3_NONE              0x00
#define IM2_PS3_EVRYSAMPLE        (0x0)
#define IM2_PS3_CROSS_TH          (0x1)
#define IM2_PS3_EXCEED_TH         (0x3)


//
// REG_PS_LED21   LED2 Current is upper nibble
//                LED1 Current is lower nibble 
//
// REG_PS_LED3    LED3 Current is lower nibble
#define LEDI_000                  0x00
#define LEDI_006                  0x01
#define LEDI_011                  0x02
#define LEDI_022                  0x03
#define LEDI_045                  0x04
#define LEDI_067                  0x05
#define LEDI_090                  0x06
#define LEDI_112                  0x07
#define LEDI_135                  0x08
#define LEDI_157                  0x09
#define LEDI_180                  0x0A
#define LEDI_202                  0x0B
#define LEDI_224                  0x0C
#define LEDI_269                  0x0D
#define LEDI_314                  0x0E
#define LEDI_359                  0x0F
#define MIN_LED_CURRENT           LEDI_006  // Do not set this to LEDI_000
#define MAX_LED_CURRENT           LEDI_359




// PARAM_CH_LIST
#define PS1_TASK                  0x01
#define PS2_TASK                  0x02
#define PS3_TASK                  0x04
#define ALS_VIS_TASK              0x10
#define ALS_IR_TASK               0x20
#define AUX_TASK                  0x40

//
// ADC Counters
// PARAM_PS_ADC_COUNTER      
// PARAM_ALSVIS_ADC_COUNTER  
// PARAM_ALSIR_ADC_COUNTER  
//
#define RECCNT_001                0x00
#define RECCNT_007                0x10
#define RECCNT_015                0x20
#define RECCNT_031                0x30
#define RECCNT_063                0x40
#define RECCNT_127                0x50
#define RECCNT_255                0x60
#define RECCNT_511                0x70

//
// Proximity LED Selection
// PARAM_PSLED12_SELECT  PS2 LED Choice is Upper Nibble
//                       PS1 LED Choice is Lower Nibble 
//
// PARAM_PSLED3_SELECT   PS3 LED Choice is Lower Nibble
//
// Each of the three PS measurements can choose whichever
// irLED to light up during the measurement, with whichever
// combination desired.
#define NO_LED                    0x00
#define LED1_EN                   0x01
#define LED2_EN                   0x02
#define LED3_EN                   0x04
#define SEL_LED1_PS1              (LED1_EN)
#define SEL_LED2_PS1              (LED2_EN)
#define SEL_LED3_PS1              (LED3_EN)
#define SEL_LED1_PS2              (LED1_EN<<4)
#define SEL_LED2_PS2              (LED2_EN<<4)
#define SEL_LED3_PS2              (LED3_EN<<4)
#define SEL_LED1_PS3              (LED1_EN)
#define SEL_LED2_PS3              (LED2_EN)
#define SEL_LED3_PS3              (LED3_EN)

//
// PARAM_PS_ENCODING  
// When these bits are set the corresponding measurement 
// will report the least significant bits of the
// ADC is used instead of the most significant bits
#define PS1_LSB                   0x10
#define PS2_LSB                   0x20
#define PS3_LSB                   0x40
#define PS_ENCODING_MASK          0x70

//
// PARAM_ALS_ENCODING 
// When these bits are set the corresponding measurement 
// will report the least significant bits of the
// ADC is used instead of the most significant bits
#define ALS_VIS_LSB               0x10
#define ALS_IR_LSB                0x20
#define AUX_LSB                   0x40
#define ALS_ENCODING_MASK         0xCF


//
// PARAM_PS_ADC_MISC         
// PARAM_ALS_VIS_ADC_MISC         
// PARAM_ALS_IR_ADC_MISC         
//
// PS_MODE_MEAS_MODE and NOT_PS_MEAS_MODE are applicable only
// for PARAM_PS_ADC_MISC.
//
//    PS_MEAS_MODE is used to perform normal Proximity measurements. 
//    While in this operatinal mode, it is possible to choose either
//    the small IR photodiode or big IR photodiode. The big IR
//    photodiode is the default for normal operation. The small IR
//    photodiode is typically not used, but it is possible to use it
//    for proximity measurements.
//
//    NOT_PS_MEAS_MODE can be applied to PARAM_PS_ADC_MISC.
//
//    This allows three PS channels to perform RAW ADC measurements 
//    on any source (no irLED driven). These measurements will have no
//    reference, and will have an offset of 0x4000 if the most 
//    significant 16 bits of the 17-bit ADC is reported. Otherwise, 
//    if the PARAM_xxx_ENCODING _LSB settings are set, then the 
//    offset will be 0x8000 (due to bit shifting). 
//
//    When performing voltage measurements (INT, LED1, LED2, TEMP), 
//    a separate VSS measurement should be subtracted from the reading.
//
//    When performing measurement with visible photodiode, first take
//    a no-led measurement and subtract away the visible light measurement.
//
//    When performing an optical measurement using either two IR 
//    photodiodes, one should subtract away the no-led measurement from 
//    the IR measurement.
//
//    Note that the subtraction ordering is significant between
//    taking visible light photodiode vs IR photodiode
//    measurement. The raw ADC reading of visible light photodiode
//    decreases with increasing light levels while the raw ADC reading 
//    of IR light photodiode increases with increasing light levels.
//
// HSIG_EN means 'high signal range enable'. The ADC would
// be able to operate with higher light levels, but at the
// expense of sensitivity. This setting can be used for 
// operation under direct sunlight.
//
// PARAM_PS_ADC_MISC, PARAM_ALSVIS_ADC_MISC and PARAM_ALSIR_ADC_MISC    
// can use HSIG_EN also.
// 
//
#define NOT_PS_MEAS_MODE          0x00 
#define PS_MEAS_MODE              0x04
#define HSIG_EN                   0x20
#define RANGE_EN                  0x20

#define ALS_IR_ADC_MISC_MASK      0x20
#define ALS_VIS_ADC_MISC_MASK     0x20

//
// ADC Mux Settings
// PARAM_PS1_ADC_MUX    See PARAM_PS_ADC_MISC also 
// PARAM_PS2_ADC_MUX    See PARAM_PS_ADC_MISC also
// PARAM_PS3_ADC_MUX    See PARAM_PS_ADC_MISC also
//
// PARAM_ALS1_ADC_MUX   MUX_ALS_VIS or MUX_NONE only
// PARAM_ALS2_ADC_MUX   MUX_ALS_IR, MUX_PS_IR or MUX_NONE only
// PARAM_ALS3_ADC_MUX   MUX_VTEMP, MUX_LED1, MUX_LED2, MUX_INT
//                      to use anything other than MUX_VTEMP, 
//                      ANA_IN_KEY should be unlocked first.
//
#define MUX_SMALL_IR              0x00
#define MUX_VIS                   0x02
#define MUX_LARGE_IR              0x03
#define MUX_NO_PHOTO_DIODE        0x06
#define MUX_VTEMP      	          0x65
#define MUX_INT      	          0x05
#define MUX_LED1      	          0x15
#define MUX_VSS      	          0x25
#define MUX_LED2      	          0x35
#define MUX_VDD      	          0x75

//
// ADC Dividers
// PARAM_PS_ADC_GAIN       
// PARAM_ALSVIS_ADC_GAIN  
// PARAM_ALSIR_ADC_GAIN 
//
#define ADC_NORM                  0x00
#define ADC_DIV2                  0x01
#define ADC_DIV4                  0x02
#define ADC_DIV8                  0x03
#define ADC_DIV16                 0x04
#define ADC_DIV32                 0x05
#define ADC_DIV64                 0x06
#define ADC_DIV128                0x07
#define ADC_DIV256                0x08
#define ADC_DIV512                0x09
#define ADC_DIV1024               0x0A
#define ADC_DIV2048               0x0B


// Hardware Key value
// REG_HW_KEY
#define HW_KEY_VAL0               0x17

// Sleep Control
// PARAM_SLEEP_CTRL 
#define SLEEP_DISABLED            0x01

// ANA_IN_KEY value
#define ANA_KEY_38                0x10
#define ANA_KEY_39                0x40
#define AMA_KEY_3A                0x62
#define ANA_KEY_3B                0x3b


#define VIS_STATIC_WINDOW_SCALE    0 /* max is 7 */ 


//
// Macro Definitions that should be modified when
// Different Si114x ADC Settings are used
//

#define IRAMB_SUNLIGHT_LOWER_THRESH               1300
#define IRAMB_HIGH_SIGNAL_UPPER_THRESH           22000
#define IRAMB_HIGH_SIGNAL_LOWER_THRESH            1300
#define IRAMB_HIGH_SENSITIVITY_UPPER_THRESH      22000
#define IRAMB_HIGH_SENSITIVITY_LOWER_THRESH       1300
#define IRAMB_LOWLIGHT_UPPER_THRESH              22000

#define VISAMB_SUNLIGHT_LOWER_THRESH               700  
#define VISAMB_HIGH_SIGNAL_UPPER_THRESH          22000
#define VISAMB_HIGH_SIGNAL_LOWER_THRESH           5000
#define VISAMB_HIGH_SENSITIVITY_UPPER_THRESH     22000
#define VISAMB_HIGH_SENSITIVITY_LOWER_THRESH      5000
#define VISAMB_LOWLIGHT_UPPER_THRESH             22000

#define IR_SCALING_SUNLIGHT                       2940 // 1*16*12.5 // 3058 1*16*13*14.7   
#define IR_SCALING_HIGH_SIGNAL                     200 // 1*16*12.5 //  208 1*16*13    
#define IR_SCALING_HIGH_SENSITIVITY                 16 // 1*16
#define IR_SCALING_LOWLIGHT                          1 // 1

#define VIS_SCALING_SUNLIGHT                       470   
#define VIS_SCALING_HIGH_SIGNAL                     16     
#define VIS_SCALING_HIGH_SENSITIVITY                 4   
#define VIS_SCALING_LOWLIGHT                         1    



#endif
