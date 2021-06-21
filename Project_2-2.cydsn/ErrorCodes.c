/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    ErrorCodes.c: source file

    Functions to check the different errors, in the UART_DEBUG is printed at which
    error we are referring to, to better understand and solve the problem
 
 * =============================================================================
*/

#include "cytypes.h"
#include "ErrorCodes.h"
#include "stdio.h"
#include "project.h"

// Function for error checking, depending on the error (input) we obtain different responses.
void error_check (ErrorCode error){   
    
    if(error != 0) {
        PWM_LED_WriteCompare(50); // Connection error, the blue led blinks
        CyGlobalIntDisable;
            for(;;){
                UART_BT_PutChar(0xE); // Communication with GUI to display the error as string
            }
    }
    
    // List of all the cases of different errors
    switch(error){
    case NO_ERROR:    
       PWM_LED_WriteCompare(99); // No error generated, the blue led is on 
       break;
    case I2C_READ_REGISTER_ERROR:
        UART_DEBUG_PutString("\nError in reading I2C register");
       break;
    case I2C_WRITE_REGISTER_ERROR:
        UART_DEBUG_PutString("\nError in writing I2C register");
        break;
    case I2C_READ_REGISTER_MULTI_ERROR:
        UART_DEBUG_PutString("\nError in reading multiple I2C registers, check FIFO_read function");
         break;
    case I2C_WRITE_REGISTER_MULTI_ERROR:
        UART_DEBUG_PutString("\nError in writing multiple I2C registers");
        break;
    case DATARATE_SET_FAIL:
        UART_DEBUG_PutString("\nError in setting data rate, check set_datarate function");
        break;
    case FSR_SET_FAIL:
        UART_DEBUG_PutString("\nError in setting fsr, check set_fsr function");
        break;
    case FIFO_SET_FAIL:
        UART_DEBUG_PutString("\nError in setting FIFO, check FIFO_set function");   
        break;
    }
}

/* [] END OF FILE */
