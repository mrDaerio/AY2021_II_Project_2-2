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
    if( error == NO_ERROR ) {
        //UART_DEBUG_PutString("No error");
    }
    else {
        UART_DEBUG_PutString("Error");
    }
}

/* [] END OF FILE */
