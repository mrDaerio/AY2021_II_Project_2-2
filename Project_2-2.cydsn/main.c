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
    ErrorCode error = get_reg(LIS3DH_WHO_AM_I_REG_ADDR, &whoami_reg);
    error_check(error);
    sprintf(message, "WHO AM I: %d\n", whoami_reg);
    UART_DEBUG_PutString(message);
    
    /*      I2C Master Read - STATUS Register       */
    uint8_t status_reg;
    error = get_reg(LIS3DH_STATUS_REG, &status_reg);
    error_check(error);
    sprintf(message, "STATUS REG: %d\n", status_reg);
    UART_DEBUG_PutString(message);
    
    // Enable data rate
    uint8_t ctrl_reg1;
    ctrl_reg1 = 0b01110111;
    error = set_reg (LIS3DH_CTRL_REG1, ctrl_reg1);
    error_check(error);
   
    int16_t X_data;
    uint8_t X_LSB, X_MSB;
    
    for(;;)
    {
        //READ LSB X
        error = get_reg(LIS3DH_REG_OUT_X_L, &X_LSB);
        error_check(error);
        
        //READ MSB X
        error = get_reg(LIS3DH_REG_OUT_X_H, &X_MSB);
        error_check(error);
        
        //merge the two bytes
        X_data = (X_MSB<<8) | X_LSB;
        X_data = X_data >> 4;
        sprintf(message, "X-data: %d\n", X_data);
        UART_DEBUG_PutString(message);
    }
}

/* [] END OF FILE */
