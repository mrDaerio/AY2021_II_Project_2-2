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

#include "LIS3DH.h"


ErrorCode set_reg (uint8_t reg, uint8_t value_reg)
{
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 
                                                   reg,
                                                   value_reg);
    return error;
}

ErrorCode get_reg (uint8_t reg, uint8_t* value_reg)
{
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  reg,
                                                  value_reg);
    return error;
}

ErrorCode set_datarate (lis3dh_dataRate_t val)
{
    uint8_t curr;
    ErrorCode err = get_reg(LIS3DH_REG_CTRL1, &curr);
    if(err == ERROR) return err;
    val = (val<<4);
    curr &= 0b00001111;
    val |= curr;
    err = set_reg(LIS3DH_REG_CTRL1, val);
    return err;
}

ErrorCode FIFO_set(uint8_t val, lis3dh_fifo_mode_t mode)
{   
    //Enables or disables FIFO
    uint8_t curr;
    ErrorCode err = get_reg(LIS3DH_REG_CTRL5, &curr);
    if(err == ERROR) return err;
    curr &= 0b10111111;
    val = (val<<6);
    val |= curr;
    err = set_reg(LIS3DH_REG_CTRL5, val);
    if(err == ERROR) return err;
   
    //set FIFO mode
    err = get_reg(LIS3DH_REG_FIFOCTRL, &curr);
    if(err == ERROR) return err;
    curr &= 0b00111111;
    mode = (mode<<6);
    mode |= curr;
    err = set_reg(LIS3DH_REG_FIFOCTRL, mode);
    
    //overrun interrupt enable
    err = get_reg(LIS3DH_REG_CTRL3, &curr);
    if(err == ERROR) return err;
    curr &= 0b11111101;
    val = (val<<1);
    val |= curr;
    err = set_reg(LIS3DH_REG_CTRL3, val);
    return err;    
    
}

/* [] END OF FILE */
