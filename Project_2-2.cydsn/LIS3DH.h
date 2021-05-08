/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef __LIS3DH_H
    #define __LIS3DH_H
    
    #include "cytypes.h"
    #include "ErrorCodes.h"
    #include "I2C_Interface.h"

    /**
    *   \brief 7-bit I2C address of the slave device.
    */
    #define LIS3DH_DEVICE_ADDRESS 0x18

    /**
    *   \brief Address of the WHO AM I register
    */
    #define LIS3DH_WHO_AM_I_REG_ADDR 0x0F
    
    /**
    *   \brief WHOAMI return value
    */
    #define LIS3DH_WHOAMI_RETVAL     0x33

    /**
    *   \brief Address of the Status register
    */
    #define LIS3DH_STATUS_REG 0x27

    /**
    *   \brief Address of the Control register
    */
    #define LIS3DH_CTRL_REG0 0x1E
    #define LIS3DH_CTRL_REG1 0x20
    #define LIS3DH_CTRL_REG2 0x21
    #define LIS3DH_CTRL_REG3 0x22
    #define LIS3DH_CTRL_REG4 0x23
    #define LIS3DH_CTRL_REG5 0x24
    #define LIS3DH_CTRL_REG6 0x25

    /**
    *   \brief Hex value to set normal mode to the accelerator
    */
    #define LIS3DH_NORMAL_MODE_CTRL_REG1 0x47
    
    /**
    *   \brief Hex value to set normal mode (powered off)
    **/
    #define LIS3DH_NORMAL_MODE_OFF_CTRL_REG1 0x07

    /**
    *   \brief  Address of the Temperature Sensor Configuration register
    */
    #define LIS3DH_TEMP_CFG_REG 0x1F 

    #define LIS3DH_TEMP_CFG_REG_ACTIVE 0xC0

    

    #define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x80

    /**
    *   \brief Address of the ADC output LSB register
    */
    #define LIS3DH_OUT_ADC_3L 0x0C

    /**
    *   \brief Address of the ADC output MSB register
    */
    #define LIS3DH_OUT_ADC_3H 0x0D
    
    #define LIS3DH_REG_OUT_X_L 0x28 /**< X-axis acceleration data. Low value */
    #define LIS3DH_REG_OUT_X_H 0x29 /**< X-axis acceleration data. High value */
    #define LIS3DH_REG_OUT_Y_L 0x2A /**< Y-axis acceleration data. Low value */
    #define LIS3DH_REG_OUT_Y_H 0x2B /**< Y-axis acceleration data. High value */
    #define LIS3DH_REG_OUT_Z_L 0x2C /**< Z-axis acceleration data. Low value */
    #define LIS3DH_REG_OUT_Z_H 0x2D /**< Z-axis acceleration data. High value */
    
    #define FIFO_CTRL_REG 0x2E
    
    /** A structure to represent scales **/
    typedef enum {
      LIS3DH_RANGE_16_G = 0b11, // +/- 16g
      LIS3DH_RANGE_8_G = 0b10,  // +/- 8g
      LIS3DH_RANGE_4_G = 0b01,  // +/- 4g
      LIS3DH_RANGE_2_G = 0b00   // +/- 2g (default value)
    } lis3dh_range_t;

    /** A structure to represent axes **/
    typedef enum {
      LIS3DH_AXIS_X = 0x0,
      LIS3DH_AXIS_Y = 0x1,
      LIS3DH_AXIS_Z = 0x2,
    } lis3dh_axis_t;

    /** Used with register 0x2A (LIS3DH_REG_CTRL_REG1) to set bandwidth **/
    typedef enum {
      LIS3DH_DATARATE_400_HZ = 0b0111, //  400Hz
      LIS3DH_DATARATE_200_HZ = 0b0110, //  200Hz
      LIS3DH_DATARATE_100_HZ = 0b0101, //  100Hz
      LIS3DH_DATARATE_50_HZ = 0b0100,  //   50Hz
      LIS3DH_DATARATE_25_HZ = 0b0011,  //   25Hz
      LIS3DH_DATARATE_10_HZ = 0b0010,  // 10 Hz
      LIS3DH_DATARATE_1_HZ = 0b0001,   // 1 Hz
      LIS3DH_DATARATE_POWERDOWN = 0,
      LIS3DH_DATARATE_LOWPOWER_1K6HZ = 0b1000,
      LIS3DH_DATARATE_LOWPOWER_5KHZ = 0b1001,
    } lis3dh_dataRate_t;
    
        typedef enum {
      BYPASS_MODE = 0x0,
      FIFO_MODE = 0x1,
      STREAM_MODE = 0x2,
      STREAM_TO_FIFO = 0x3,      
    } lis3dh_fifo_mode_t;
    
    ErrorCode set_reg (uint8_t reg, uint8_t value_reg);
    ErrorCode get_reg (uint8_t reg, uint8_t* value_reg);
    ErrorCode set_datarate (lis3dh_dataRate_t val);
    
#endif

/* [] END OF FILE */
