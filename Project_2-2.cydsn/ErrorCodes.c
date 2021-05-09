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

#include "cytypes.h"
#include "ErrorCodes.h"
#include "stdio.h"
#include "project.h"

void error_check (ErrorCode error)
{   
    switch(error){
    case NO_ERROR:    ///< No error generated
       break;
    case I2C_READ_REGISTER_ERROR:
        UART_DEBUG_PutString("\nError in reading I2C register");
       break;
    case I2C_WRITE_REGISTER_ERROR:
        UART_DEBUG_PutString("\nError in writing I2C register");
        break;
    case I2C_READ_REGISTER_MULTI_ERROR:
        UART_DEBUG_PutString("\nError in reading multiple I2C registers ");
         break;
    case I2C_WRITE_REGISTER_MULTI_ERROR:
        UART_DEBUG_PutString("\nError in writing multiple I2C registers");
        break;
    case DATARATE_SET_FAIL:
        UART_DEBUG_PutString("\nError in setting data rate, check set_datarate function");
        break;
    case FIFO_SET_FAIL:
        UART_DEBUG_PutString("\nError in setting FIFO, check FIFO_set function");   
        break;
    }
    
}

/* [] END OF FILE */
