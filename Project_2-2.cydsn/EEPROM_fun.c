#include "EEPROM_fun.h"
#include "project.h"
#include "cytypes.h"
#include "stdio.h"


// Function to read Sample Rate from the EEPROM, or write the default value if the first time
uint8_t Sample_Rate_Read(){
    uint8_t datarate;

    // Check HEAD
    uint8_t head = EEPROM_ReadByte(SAMPLE_RATE_ADDRESS-1);
    
    // Check TAIL
    uint8_t tail = EEPROM_ReadByte(SAMPLE_RATE_ADDRESS+1);
    
    // If the EEPROM has already been written, read the value
    if(head == SAMPLE_RATE_HEAD && tail == SAMPLE_RATE_TAIL){
       datarate = EEPROM_ReadByte(SAMPLE_RATE_ADDRESS);
    }
    
    // If it is the first time, write the initialization values
    else {
        // uint8_t error = EEPROM_UpdateTemperature();
        EEPROM_WriteByte(SAMPLE_RATE_HEAD, SAMPLE_RATE_ADDRESS-1);
        EEPROM_WriteByte(SAMPLE_RATE, SAMPLE_RATE_ADDRESS);
        EEPROM_WriteByte(SAMPLE_RATE_TAIL, SAMPLE_RATE_ADDRESS+1);
        datarate = EEPROM_ReadByte(SAMPLE_RATE_ADDRESS);  
    } 
    
    return datarate;
}

// Function to read Full Scale from the EEPROM, or write the default value if the first time
uint8_t Full_Scale_Read(){
    uint8_t fsc;

    // Check HEAD
    uint8_t head = EEPROM_ReadByte(FULL_SCALE_ADDRESS-1);
    
    // Check TAIL
    uint8_t tail = EEPROM_ReadByte(FULL_SCALE_ADDRESS+1);
    
    // If the EEPROM has already been written, read the value
    if(head == FULL_SCALE_HEAD && tail == FULL_SCALE_TAIL){
       fsc = EEPROM_ReadByte(FULL_SCALE_ADDRESS);
    }
    
    // If it is the first time, write the initialization values
    else {
        // uint8_t error = EEPROM_UpdateTemperature();
        EEPROM_WriteByte(FULL_SCALE_HEAD, FULL_SCALE_ADDRESS-1);
        EEPROM_WriteByte(SAMPLE_RATE, FULL_SCALE_ADDRESS);
        EEPROM_WriteByte(SAMPLE_RATE_TAIL, FULL_SCALE_ADDRESS+1);
        fsc = EEPROM_ReadByte(FULL_SCALE_ADDRESS);  
    } 
    
    return fsc;
}

/* [] END OF FILE */
