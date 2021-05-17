#ifndef __INTERRUPT_ROUTINES_H
    
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
   
    CY_ISR_PROTO(Custom_ISR_FIFO);
    CY_ISR_PROTO(Custom_ISR_RXBT);
    
    uint8_t datarate;
    uint8_t fsc;
    
#endif
/* [] END OF FILE */
