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

#define HEAD 0xA0
#define TAIL 0xC0

#define BUFFER_SIZE 8

#define XH 1
#define XL 2
#define YH 3
#define YL 4
#define ZH 5
#define ZL 6

uint8_t flag = 0;
int16_t x_buffer[FIFO_SIZE], y_buffer[FIFO_SIZE], z_buffer[FIFO_SIZE];
uint8_t buffer[BUFFER_SIZE];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_Peripheral_Start();
    UART_DEBUG_Start();
    UART_BT_Start();
    isr_FIFO_StartEx(Custom_ISR_FIFO);
    isr_BT_StartEx(Custom_ISR_RXBT);
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
    error = get_reg(LIS3DH_REG_STATUS2, &status_reg);
    error_check(error);
    sprintf(message, "STATUS REG: %d\n", status_reg);
    UART_DEBUG_PutString(message);
    
    // Accelerometer powered down
    error = set_datarate(LIS3DH_DATARATE_POWERDOWN);
    error_check(error);
    
    // Enables FIFO
    error = FIFO_set(1, FIFO_MODE);
    error_check(error);

    buffer[0] = HEAD;
    buffer[7] = TAIL;
    
    if (!I2C_Peripheral_IsDeviceConnected(LIS3DH_DEVICE_ADDRESS))
        PWM_LED_WriteCompare(LED_BLINK);
    else
        PWM_LED_WriteCompare(LED_ON);

    for(;;)
    {      
        if(flag){
            flag = 0;
            for(int i=0; i<FIFO_SIZE; i++){
                
                buffer[XH] = x_buffer[i] >> 8;
                buffer[XL] = x_buffer[i];
                buffer[YH] = y_buffer[i] >> 8;
                buffer[YL] = y_buffer[i];
                buffer[ZH] = z_buffer[i] >> 8;
                buffer[ZL] = z_buffer[i];
                
                UART_DEBUG_PutArray(buffer, 8);
                UART_BT_PutArray(buffer, BUFFER_SIZE);

            }
        }       
    }
}

/* [] END OF FILE */
