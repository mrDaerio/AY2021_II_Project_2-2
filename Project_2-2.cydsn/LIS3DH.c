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

extern uint16_t x_buffer[FIFO_SIZE], y_buffer[FIFO_SIZE], z_buffer[FIFO_SIZE];
extern uint8_t flag;

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
    if(err) return DATARATE_SET_FAIL;
    val = (val<<4);
    curr &= ~LIS3DH_REG_CTRL1_DATA_RATE_BITS_MASK;
    val |= curr;
    err = set_reg(LIS3DH_REG_CTRL1, val);
    return err;
}

ErrorCode FIFO_set(uint8_t val, lis3dh_fifo_mode_t mode)
{  
    //Enables or disables FIFO
    uint8_t curr;
    char message[50] = {'\0'};
    ErrorCode err = get_reg(LIS3DH_REG_CTRL5, &curr);
    /*
    sprintf(message, "\nLIS3DH_REG_CTRL5 BEFORE: %d", curr);
        UART_DEBUG_PutString(message);
    */
    if(err) return FIFO_SET_FAIL;
    curr &= ~LIS3DH_REG_CTRL5_FIFO_SET_BITS_MASK;
    curr |= (val<<6);
    err = set_reg(LIS3DH_REG_CTRL5, curr);
    /*
    err = get_reg(LIS3DH_REG_CTRL5, &curr);
    sprintf(message, "\nLIS3DH_REG_CTRL5 AFTER: %d", curr);
        UART_DEBUG_PutString(message);
    */
    if(err) return FIFO_SET_FAIL;
   
    //set FIFO mode
    err = get_reg(LIS3DH_REG_FIFOCTRL, &curr);
    /*
        sprintf(message, "\nLIS3DH_REG_FIFOCTRL BEFORE: %d", curr);
        UART_DEBUG_PutString(message);
    */
    if(err) return FIFO_SET_FAIL;
    curr &= ~LIS3DH_REG_FIFOCTRL_FIFO_SET_BITS_MASK;
    mode = (mode<<6);
    mode |= curr;
    err = set_reg(LIS3DH_REG_FIFOCTRL, mode);
    /*
        err = get_reg(LIS3DH_REG_FIFOCTRL, &curr);
    sprintf(message, "\nLIS3DH_REG_FIFOCTRL AFTER: %d should be %d", curr, mode);
        UART_DEBUG_PutString(message);
    */
    //overrun interrupt enable/disable
    err = get_reg(LIS3DH_REG_CTRL3, &curr);
    /*
    sprintf(message, "\nLIS3DH_REG_CTRL3 BEFORE: %d", curr);
    UART_DEBUG_PutString(message);
    */
    if(err) return FIFO_SET_FAIL;
    curr &= ~LIS3DH_REG_CTRL3_FIFO_INT1_BITS_MASK;
    val = (val<<1);
    val |= curr;
    err = set_reg(LIS3DH_REG_CTRL3, val);
    /*
        err = get_reg(LIS3DH_REG_CTRL3, &curr);
        sprintf(message, "\nLIS3DH_REG_CTRL3 AFTER: %d should be %d", curr, val);
        UART_DEBUG_PutString(message);
    */
    return err;    
    
}

void FIFO_read()
{
    ErrorCode error;
    int16_t X_data, Y_data, Z_data;
    uint8_t X_LSB, X_MSB, Y_LSB, Y_MSB, Z_LSB, Z_MSB;
    
    for(int i = 0; i<FIFO_SIZE; i++){
        //READ LSB X
        error = get_reg(LIS3DH_REG_OUT_X_L, &X_LSB);
        error_check(error);
        
        //READ MSB X
        error = get_reg(LIS3DH_REG_OUT_X_H, &X_MSB);
        error_check(error);
        
        //READ LSB Y
        error = get_reg(LIS3DH_REG_OUT_Y_L, &Y_LSB);
        error_check(error);
        
        //READ MSB Y
        error = get_reg(LIS3DH_REG_OUT_Y_H, &Y_MSB);
        error_check(error);
        
        //READ LSB Z
        error = get_reg(LIS3DH_REG_OUT_Z_L, &Z_LSB);
        error_check(error);
        
        //READ MSB Z
        error = get_reg(LIS3DH_REG_OUT_Z_H, &Z_MSB);
        error_check(error);
        
         //merge the two bytes
        X_data = (X_MSB<<8) | X_LSB;
        X_data = X_data >> 4;       
        Y_data = (Y_MSB<<8) | Y_LSB;
        Y_data = Y_data >> 4;        
        Z_data = (Z_MSB<<8) | Z_LSB;
        Z_data = Z_data >> 4;
        
        x_buffer[i] = X_data;
        y_buffer[i] = Y_data;
        z_buffer[i] = Z_data;
        
    }
    
    //Enter bypass mode to reset FIFO content
    error = FIFO_set(1, BYPASS_MODE);
    error = FIFO_set(1, FIFO_MODE);
    flag = 1;
    
}

/* [] END OF FILE */
