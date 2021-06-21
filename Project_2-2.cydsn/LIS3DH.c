/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    LIS3DH.c: source file 

    This file contains all the functions used to operate with the registers of the LIS3DH
 
 * =============================================================================
*/

#include "LIS3DH.h"
#include "project.h"
#include "stdio.h"

/* Definition of the three different buffers for the three respective axis, with associated the 
dimension of the buffer (defined in the file LIS3DH.h */
extern int16_t x_buffer[FIFO_SIZE], y_buffer[FIFO_SIZE], z_buffer[FIFO_SIZE];

// Flag to check if the FIFO buffer is full
extern uint8_t flag;

#define XL 0
#define XH 1
#define YL 2
#define YH 3
#define ZL 4
#define ZH 5

/* Function used to set a general register of the LIS3DH with a specific value.
   
   This function takes in input two parameters:
   - Register 
   - Value to set the register

   If the operation fails, it returns the respective error, in order to understand
   the problem */

ErrorCode set_reg (uint8_t reg, uint8_t value_reg)
{
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 
                                                   reg,
                                                   value_reg);
    return error;
}


/* Function used to read a general register of the LIS3DH.
   
   This function takes in input two parameters:
   - Register 
   - Value to set the register (pointer)

   If the operation fails, it returns the respective error, in order to understand
   the problem */

ErrorCode get_reg (uint8_t reg, uint8_t* value_reg)
{
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  reg,
                                                  value_reg);
    return error;
}


/* Function used to set only specific bits of a general register of the LIS3DH with a 
   specific value.
   The function implies the usage of a mask to select the required bits.
   
   This function takes in input three parameters:
   - Register 
   - Mask for the bits
   - Value to set the register

   If the operation fails, it returns the respective error, in order to understand
   the problem */

ErrorCode set_reg_masked_only (uint8_t reg, uint8_t mask, uint8_t value_reg)
{
    // Read register
    uint8_t curr; // Current value of the register
    ErrorCode err = get_reg(reg, &curr);
    if (err) return err;

    // Reset mask bits preserving the others
    curr &= ~mask;

    /* Reset unused pattern bits preserving the useful ones (not necessary if value_reg pattern is correct)
    maybe use as check? e.g. if (value_reg == value_reg & mask) -> correct pattern */
    value_reg &= mask;

    // Set pattern bits into the register
    curr |= value_reg;
    err = set_reg(reg,curr);

    return err;
}


/* Function used to set the datarate of the LIS3DH with a specific value.
   
   This function takes in input one parameter:
   - Value of the datarate

   The value of the register and the mask have already been selected.
   If the operation fails, it returns the respective error, in order to understand
   the problem */

ErrorCode set_datarate (lis3dh_dataRate_t val)
{
    ErrorCode err = set_reg_masked_only(LIS3DH_REG_CTRL1,
                                        LIS3DH_REG_CTRL1_DATA_RATE_BITS_MASK,
                                        val<<4);
    return err ? DATARATE_SET_FAIL : NO_ERROR;
}


/* Function used to set the full scale range of the LIS3DH with a specific value.
   
   This function takes in input one parameter:
   - Value of the full scale range

   The value of the register and the mask have already been selected.
   If the operation fails, it returns the respective error, in order to understand
   the problem */

ErrorCode set_range (lis3dh_range_t val)
{
    ErrorCode err = set_reg_masked_only(LIS3DH_REG_CTRL4,
                                        LIS3DH_REG_CTRL1_RANGE_BITS_MASK,
                                        val<<4);
    return err ? FSR_SET_FAIL : NO_ERROR;
}


/* Function used to activate and set the FIFO.
   
   This function takes in input two parameter:
   - Value used to activate/deactivate the FIFO (Reg 5) and to activate/deactivate
     the overrun interrupt (Reg 3), 1/0
   - Value to set the FIFO mode that we want to use

   The value of the register and the mask have already been selected.
   If the operation fails, it returns the respective error, in order to understand
   the problem */

ErrorCode FIFO_set(uint8_t val, lis3dh_fifo_mode_t mode)
{
    // FIFO Enable/Disable 
    ErrorCode err = set_reg_masked_only(LIS3DH_REG_CTRL5,
                                        LIS3DH_REG_CTRL5_FIFO_SET_BITS_MASK,
                                        val<<6);
    if(err) return FIFO_SET_FAIL;

    // Set FIFO mode
    err = set_reg_masked_only(LIS3DH_REG_FIFOCTRL,
                              LIS3DH_REG_FIFOCTRL_FIFO_MODE_BITS_MASK,
                              mode<<6);
    if (err) return FIFO_SET_FAIL;

    // Overrun interrupt Enable/Disable
    err = set_reg_masked_only(LIS3DH_REG_CTRL3,
                              LIS3DH_REG_CTRL3_FIFO_INT1_BITS_MASK,
                              val<<1);

    return err ? FIFO_SET_FAIL : NO_ERROR;
}


/* Function used to read the samples in the FIFO, and put them in the respective
   buffers according to the axis.
   
   If the operation fails, it returns the respective error, in order to understand
   the problem */

void FIFO_read()
{
    ErrorCode error;
    int16_t X_data, Y_data, Z_data; // The final data is 2 bytes
    uint8_t fifo[6];
    
    for(int i = 0; i<FIFO_SIZE; i++){
        error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_REG_OUT_X_L,
                                                 6,  // 2 bytes for every coordinate
                                                 fifo);
        error_check(error);
        
        // Merge the two bytes for X
        X_data = (fifo[XH]<<8) | fifo[XL];
        X_data = X_data >> 4;   // We're reading a 12bit datum, left adjusted    
        
        // Merge the two bytes for Y
        Y_data = (fifo[YH]<<8) | fifo[YL];
        Y_data = Y_data >> 4;   // We're reading a 12bit datum, left adjusted   
        
        // Merge the two bytes for Z
        Z_data = (fifo[ZH]<<8) | fifo[ZL];
        Z_data = Z_data >> 4;   // We're reading a 12bit datum, left adjusted
        
        // Fill up the three buffer with the complete values
        x_buffer[i] = X_data;
        y_buffer[i] = Y_data;
        z_buffer[i] = Z_data; 
    }
    
    // Enter bypass mode to reset FIFO content
    error = FIFO_set(1, BYPASS_MODE);
    error = FIFO_set(1, FIFO_MODE);
    
    // Raise flag when FIFO is full
    flag = 1;
    
}

/* [] END OF FILE */
