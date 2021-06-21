/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    InterruptRoutines.h: header file
 
 * =============================================================================
*/
    
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
