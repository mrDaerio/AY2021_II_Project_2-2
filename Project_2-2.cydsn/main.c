/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    main.c: source file
 
 * =============================================================================
*/

#include "project.h"
#include "stdio.h"
#include "I2C_Interface.h"
#include "LIS3DH.h"
#include "ErrorCodes.h"
#include "InterruptRoutines.h"

// Definition of led value for blinking
#define LED_ON 99
#define LED_BLINK 50

// Definition of head and tail for data packets
#define HEAD 0xA0
#define TAIL 0xC0

// Definition of buffer size
#define BUFFER_SIZE 194

uint8_t flag = 0;
int16_t x_buffer[FIFO_SIZE], y_buffer[FIFO_SIZE], z_buffer[FIFO_SIZE];
uint8_t buffer[BUFFER_SIZE];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Start all components
    I2C_Peripheral_Start();
    UART_DEBUG_Start();
    UART_BT_Start();
    isr_FIFO_StartEx(Custom_ISR_FIFO);
    isr_BT_StartEx(Custom_ISR_RXBT);
    PWM_LED_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    if (!I2C_Peripheral_IsDeviceConnected(LIS3DH_DEVICE_ADDRESS))
        PWM_LED_WriteCompare(LED_BLINK);
    else
        PWM_LED_WriteCompare(LED_ON);
        
    CyDelay(10);
        
    // Accelerometer powered down
    ErrorCode error = set_datarate(LIS3DH_DATARATE_POWERDOWN);
    error_check(error);
    
    // Enables FIFO
    error = FIFO_set(1, FIFO_MODE);
    error_check(error);

    buffer[0] = HEAD;
    buffer[BUFFER_SIZE-1] = TAIL;
    
    for(;;)
    {      
        if(flag){
            flag = 0;
            for(int i=1; i <= FIFO_SIZE; i++) {
                    buffer[2*i - 1] = x_buffer[i-1] >> 8;
                    buffer[2*i] = x_buffer[i-1];
                    buffer[2*i + 2*FIFO_SIZE - 1] = y_buffer[i-1] >> 8;
                    buffer[2*i + 2*FIFO_SIZE] = y_buffer[i-1];
                    buffer[2*(i + 2*FIFO_SIZE) - 1] = z_buffer[i-1] >> 8;
                    buffer[2*(i + 2*FIFO_SIZE)] = z_buffer[i-1];
            }
            UART_BT_PutArray(buffer, BUFFER_SIZE);
        }    
    }
}

/* [] END OF FILE */
