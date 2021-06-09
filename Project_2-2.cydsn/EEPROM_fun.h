#ifndef __EEPROM_H
    #define __EEPROM_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    // DEFAULT PARAMETERS
    #define SAMPLE_RATE 0x06
    #define FULL_SCALE  0x00
    
    // ADDRESS PARAMETERS IN EEPROM
    #define SAMPLE_RATE_ADDRESS  1 
    #define FULL_SCALE_ADDRESS   4 

    // HEAD
    #define SAMPLE_RATE_HEAD  0xA0
    #define FULL_SCALE_HEAD   0xA1
    
    // TAIL
    #define SAMPLE_RATE_TAIL  0xC0
    #define FULL_SCALE_TAIL   0xC1 
    
    // Definition of functions to retrieve values
    uint8_t Sample_Rate_Read();
    uint8_t Full_Scale_Read(); 
    
#endif
/* [] END OF FILE */
