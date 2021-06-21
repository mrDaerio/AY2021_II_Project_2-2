/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    InterruptRoutines.c: source file

    In this file are defined the interrupt routines function 
 
 * =============================================================================
*/

#include "InterruptRoutines.h"
#include "project.h"
#include "LIS3DH.h"
#include "ErrorCodes.h"
#include "EEPROM_fun.h"

CY_ISR(Custom_ISR_FIFO)
{
    Pin_INT_ClearInterrupt();
    FIFO_read();
}

CY_ISR(Custom_ISR_RXBT)
{   
    char message[50] = {'\0'};
    ErrorCode error;
    char ch_received = UART_BT_GetChar();
    switch(ch_received)
    {
        case 'v':
        // Retrieve the values saved in the EEPROM at connection
        datarate = Sample_Rate_Read();
        fsc = Full_Scale_Read();
        
        // String to be able to connect to GUI
        sprintf(message, "$$$LIS %d %d", datarate, fsc);
        UART_BT_PutString(message);
            break;

        case 's':  // Stop condition
        error = set_datarate(LIS3DH_DATARATE_POWERDOWN);
        error_check(error);
            break;

        case 'b':  // Start condition
        // Set the datarate
        error = set_datarate(datarate);
        error_check(error);
        
        // Set the full scale range
        error = set_range(fsc);
        error_check(error);
            break;
        
        // Different possibilities for the datarate        
        case '4':
        datarate = LIS3DH_DATARATE_100_HZ;
        EEPROM_WriteByte(datarate, SAMPLE_RATE_ADDRESS);
            break;
        
        case '5':
        datarate = LIS3DH_DATARATE_200_HZ;
        EEPROM_WriteByte(datarate, SAMPLE_RATE_ADDRESS);
            break;
        
        case '6':
        datarate = LIS3DH_DATARATE_400_HZ;
        EEPROM_WriteByte(datarate, SAMPLE_RATE_ADDRESS);
            break;
        
        case '7':
        datarate = LIS3DH_DATARATE_1344Hz;
        EEPROM_WriteByte(datarate, SAMPLE_RATE_ADDRESS);
            break;
        
        // Different possibilities for the full scale range
        case 'w':
        fsc = LIS3DH_RANGE_2_G;
        EEPROM_WriteByte(fsc, FULL_SCALE_ADDRESS);
            break;
        
        case 'x':
        fsc = LIS3DH_RANGE_4_G;
        EEPROM_WriteByte(fsc, FULL_SCALE_ADDRESS);
            break;
        
        case 'j':
        fsc = LIS3DH_RANGE_8_G;
        EEPROM_WriteByte(fsc, FULL_SCALE_ADDRESS);
            break;
        
        case 'k':
        fsc = LIS3DH_RANGE_16_G;
        EEPROM_WriteByte(fsc, FULL_SCALE_ADDRESS);
            break;
        default:
            break;
    }
}    

/* [] END OF FILE */
