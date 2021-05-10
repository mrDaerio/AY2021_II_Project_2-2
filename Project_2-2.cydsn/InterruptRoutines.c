#include "InterruptRoutines.h"
#include "project.h"
#include "LIS3DH.h"


CY_ISR(Custom_ISR_FIFO)
{
    UART_DEBUG_PutString("\nWE ARE IN THE INTERRUPT ROUTINE\n");
    Pin_INT_ClearInterrupt();
    FIFO_read();
    
}

    

/* [] END OF FILE */
