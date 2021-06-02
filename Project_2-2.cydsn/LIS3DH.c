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
#include "project.h"
#include "stdio.h"

extern int16_t x_buffer[FIFO_SIZE], y_buffer[FIFO_SIZE], z_buffer[FIFO_SIZE];
extern uint8_t flag;

#define XL 0
#define XH 1
#define YL 2
#define YH 3
#define ZL 4
#define ZH 5

// Function to set a general register of the LIS3DH with value
ErrorCode set_reg (uint8_t reg, uint8_t value_reg)
{
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 
                                                   reg,
                                                   value_reg);
    return error;
}

// Function to read a general register of the LIS3DH 
ErrorCode get_reg (uint8_t reg, uint8_t* value_reg)
{
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  reg,
                                                  value_reg);
    return error;
}

// Function to set only specific bits of a general register with a mask
ErrorCode set_reg_masked_only (uint8_t reg, uint8_t mask, uint8_t value_reg)
{
    //read register
    uint8_t curr;
    ErrorCode err = get_reg(reg, &curr);
    if (err) return err;

    //reset mask bits preserving the others
    curr &= ~mask;

    //reset unused pattern bits preserving the useful ones (not necessary if value_reg pattern is correct)
    //maybe use as check? e.g. if (value_reg == value_reg & mask) -> correct pattern
    value_reg &= mask;

    //set pattern bits into the register
    curr |= value_reg;
    err = set_reg(reg,curr);

    return err;
}

// Function to set the datarate of the LIS3DH
ErrorCode set_datarate (lis3dh_dataRate_t val)
{
    ErrorCode err = set_reg_masked_only(LIS3DH_REG_CTRL1,
                                        LIS3DH_REG_CTRL1_DATA_RATE_BITS_MASK,
                                        val<<4);
    return err ? DATARATE_SET_FAIL : NO_ERROR;
}

// Function to set the full scale range of the LIS3DH
ErrorCode set_range (lis3dh_range_t val)
{
    ErrorCode err = set_reg_masked_only(LIS3DH_REG_CTRL4,
                                        LIS3DH_REG_CTRL1_RANGE_BITS_MASK,
                                        val<<4);
    return err ? FSR_SET_FAIL : NO_ERROR;
}

// Function to activate and set the FIFO
ErrorCode FIFO_set(uint8_t val, lis3dh_fifo_mode_t mode)
{
    //Enables or disables FIFO
    ErrorCode err = set_reg_masked_only(LIS3DH_REG_CTRL5,
                                        LIS3DH_REG_CTRL5_FIFO_SET_BITS_MASK,
                                        val<<6);
    if(err) return FIFO_SET_FAIL;

    //set FIFO mode
    err = set_reg_masked_only(LIS3DH_REG_FIFOCTRL,
                              LIS3DH_REG_FIFOCTRL_FIFO_MODE_BITS_MASK,
                              mode<<6);
    if (err) return FIFO_SET_FAIL;

    //overrun interrupt enable/disable
    err = set_reg_masked_only(LIS3DH_REG_CTRL3,
                              LIS3DH_REG_CTRL3_FIFO_INT1_BITS_MASK,
                              val<<1);


    return err ? FIFO_SET_FAIL : NO_ERROR;

}

// Function to read the samples in the FIFO
void FIFO_read()
{
    ErrorCode error;
    int16_t X_data, Y_data, Z_data;
    uint8_t fifo[6];
    
    for(int i = 0; i<FIFO_SIZE; i++){
        error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_REG_OUT_X_L,
                                        6,  //2 bytes for every coordinate
                                        fifo);
        error_check(error);
        // Merge the two bytes
        X_data = (fifo[XH]<<8) | fifo[XL];
        X_data = X_data >> 4;   //We're reading a 10bit datum, left adjusted    
        Y_data = (fifo[YH]<<8) | fifo[YL];
        Y_data = Y_data >> 4;        
        Z_data = (fifo[ZH]<<8) | fifo[ZL];
        Z_data = Z_data >> 4;
        
        x_buffer[i] = X_data;
        y_buffer[i] = Y_data;
        z_buffer[i] = Z_data;
        
        
    }
    
    // Enter bypass mode to reset FIFO content
    error = FIFO_set(1, BYPASS_MODE);
    error = FIFO_set(1, FIFO_MODE);
    flag = 1;
    
}

/* [] END OF FILE */
