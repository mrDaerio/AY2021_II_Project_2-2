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

#include "project.h"
#include "stdio.h"
#include "I2C_Interface.h"
#include "LIS3DH.h"
#include "ErrorCodes.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_Peripheral_Start();
    UART_DEBUG_Start();
    
    char message[50] = {'\0'};
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    /*      I2C Master Read - WHOAMI Register       */
    uint8_t whoami_reg;
    ErrorCode error = get_reg(LIS3DH_REG_WHOAMI, &whoami_reg);
    error_check(error);
    sprintf(message, "WHO AM I: %d\n", whoami_reg);
    UART_DEBUG_PutString(message);
    
    /*      I2C Master Read - STATUS Register       */
    uint8_t status_reg;
    error = get_reg(LIS3DH_REG_STATUS1, &status_reg);
    error_check(error);
    sprintf(message, "STATUS REG: %d\n", status_reg);
    UART_DEBUG_PutString(message);
    
    // Enable accelerometer
    error = set_datarate(LIS3DH_DATARATE_400_HZ);
    error_check(error);
    
    int16_t X_data, Y_data, Z_data;
    uint8_t X_LSB, X_MSB, Y_LSB, Y_MSB, Z_LSB, Z_MSB;
    
    for(;;)
    {
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
        sprintf(message, "X-data: %d\n", X_data);
        UART_DEBUG_PutString(message);
        
        Y_data = (Y_MSB<<8) | Y_LSB;
        Y_data = Y_data >> 4;
        sprintf(message, "Y-data: %d\n", Y_data);
        UART_DEBUG_PutString(message);
        
        Z_data = (Z_MSB<<8) | Z_LSB;
        Z_data = Z_data >> 4;
        sprintf(message, "Z-data: %d\n", Z_data);
        UART_DEBUG_PutString(message);
        
        CyDelay(20);
    }
}

/* [] END OF FILE */
