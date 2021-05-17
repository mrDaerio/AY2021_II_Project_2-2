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
        case 'v':
        UART_BT_PutString("$$$LIS");
        UART_DEBUG_PutString("COMMUNICATION START\n");
            break;

        case 's':
        error = set_datarate(LIS3DH_DATARATE_POWERDOWN);
        error_check(error);
        UART_DEBUG_PutString("STOP\n");
            break;

        case 'b':
        error = set_datarate(datarate);
        error_check(error);
        UART_DEBUG_PutString("START\n");
            break;
        
        case '0':
        datarate = LIS3DH_DATARATE_1_HZ;
            break;
        
        case '1':
        datarate = LIS3DH_DATARATE_10_HZ;
            break;
        
        case '2':
        datarate = LIS3DH_DATARATE_25_HZ;
            break;
        
        case '3':
        datarate = LIS3DH_DATARATE_50_HZ;
            break;
        
        case '4':
        datarate = LIS3DH_DATARATE_100_HZ;
            break;
        
        case '5':
        datarate = LIS3DH_DATARATE_200_HZ;
            break;
        
        case '6':
        datarate = LIS3DH_DATARATE_400_HZ;
            break;
        
        case '7':
        datarate = LIS3DH_DATARATE_1344Hz;
            break;
        default:
            break;
    }
}    

/* [] END OF FILE */
