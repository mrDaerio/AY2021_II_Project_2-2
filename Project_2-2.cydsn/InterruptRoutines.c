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
        
        case '0':
        error = set_datarate(LIS3DH_DATARATE_1_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 1 Hz\n");
            break;
        
        case '1':
        error = set_datarate(LIS3DH_DATARATE_10_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 10 Hz\n");
            break;
        
        case '2':
        error = set_datarate(LIS3DH_DATARATE_25_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 25 Hz\n");
            break;
        
        case '3':
        error = set_datarate(LIS3DH_DATARATE_50_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 50 Hz\n");
            break;
        
        case '4':
        error = set_datarate(LIS3DH_DATARATE_100_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 100 Hz\n");
            break;
        
        case '5':
        error = set_datarate(LIS3DH_DATARATE_200_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 200 Hz\n");
            break;
        
        case '6':
        error = set_datarate(LIS3DH_DATARATE_400_HZ);
        error_check(error);
        UART_DEBUG_PutString("Changed at 400 Hz\n");
            break;
        
        case '7':
        error = set_datarate(LIS3DH_DATARATE_1344Hz);
        error_check(error);
        UART_DEBUG_PutString("Changed at 1344 Hz\n");
            break;
        default:
            break;
    }
}    

/* [] END OF FILE */
