#include "InterruptRoutines.h"
#include "project.h"
#include "LIS3DH.h"
#include "ErrorCodes.h"

CY_ISR(Custom_ISR_FIFO)
{
    //UART_DEBUG_PutString("\nWE ARE IN THE INTERRUPT ROUTINE\n");
    Pin_INT_ClearInterrupt();
    FIFO_read();
    
}

CY_ISR(Custom_ISR_RXBT)
{   
    ErrorCode error;
    char ch_received = UART_BT_GetChar();
    switch(ch_received)
    {
        case 'V':
        case 'v':
        UART_BT_PutString("$$$LIS");
        UART_DEBUG_PutString("COMMUNICATION START\n");
            break;
        case 'S':
        case 's':
        error = set_datarate(LIS3DH_DATARATE_POWERDOWN);
        error_check(error);
        UART_DEBUG_PutString("STOP\n");
            break;
        case 'B':
        case 'b':
        error = set_datarate(LIS3DH_DATARATE_1344Hz);
        error_check(error);
        UART_DEBUG_PutString("START\n");
            break;
        default:
            break;
    }
}    

/* [] END OF FILE */
