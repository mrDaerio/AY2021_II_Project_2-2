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
#include "InterruptRoutines.h"

#define LED_ON 99
#define LED_BLINK 50

uint8_t flag = 0;
uint8_t buffer[8];
int16_t x_buffer[FIFO_SIZE], y_buffer[FIFO_SIZE], z_buffer[FIFO_SIZE];


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_Peripheral_Start();
    UART_DEBUG_Start();
    isr_FIFO_StartEx(Custom_ISR_FIFO);
    
    PWM_LED_Start();
    
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
    //enable high res mode
    error = set_reg_masked_only(LIS3DH_REG_CTRL4,
                                        0b00001000,
                                        1<<3);
    error_check(error);
    error = get_reg(LIS3DH_REG_CTRL4, &whoami_reg);
    error_check(error);
    sprintf(message, "CTRL4: %d\n", whoami_reg);
    UART_DEBUG_PutString(message);
    //enables FIFO
    error = FIFO_set(1, FIFO_MODE);
    error_check(error);
    buffer[0] = 0xA0;
    buffer[7] = 0xC0;
    for(;;)
    {
        CyDelay(10);
        if (!I2C_Peripheral_IsDeviceConnected(LIS3DH_DEVICE_ADDRESS))
                PWM_LED_WriteCompare(LED_BLINK);
        else
            PWM_LED_WriteCompare(LED_ON);
        
        if(flag){
            flag = 0;
            for(int i=0;i<FIFO_SIZE;i++){
                buffer[1] =x_buffer[i] >> 8;
                buffer[2] = x_buffer[i];
                buffer[3] =y_buffer[i] >> 8;
                buffer[4] = y_buffer[i];
                buffer[5] =z_buffer[i] >> 8;
                buffer[6] = z_buffer[i];
                UART_DEBUG_PutArray(buffer, 8);
            }
            
        }
        
    }
}

/* [] END OF FILE */


/* [] END OF FILE */
