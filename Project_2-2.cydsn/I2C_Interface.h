/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    I2C_Interface.h: head file
 
 * =============================================================================
*/

#ifndef __I2C_Interface_H
    #define __I2C_Interface_H
    
    #include "cytypes.h"
    #include "ErrorCodes.h"
    
    // This function starts the I2C peripheral so that it is ready to work.
    ErrorCode I2C_Peripheral_Start(void);
    
    // This function stops the I2C peripheral from working.
    ErrorCode I2C_Peripheral_Stop(void);
    
    /* This function performs a complete reading operation over I2C from a single
       register.
       Takes three inputs:
       - param device_address I2C address of the device to talk to.
       - param register_address Address of the register to be read.
       - param data Pointer to a variable where the byte will be saved.
    */
    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                          uint8_t register_address,
                                          uint8_t* data);
    
    /* This function performs a complete reading operation over I2C from multiple
       registers.
       Takes four inputs:
       - param device_address I2C address of the device to talk to.
       - param register_address Address of the first register to be read.
       - param register_count Number of registers we want to read.
       - param data Pointer to an array where data will be saved.
    */
    ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                               uint8_t register_address,
                                               uint8_t register_count,
                                               uint8_t* data);
    
    /* This function performs a complete writing operation over I2C to a single 
       register.
       Takes three inputs:
       - param device_address I2C address of the device to talk to.
       - param register_address Address of the register to be written.
       - param data Data to be written
    */
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                           uint8_t register_address,
                                           uint8_t data);
    
    /* This function performs a complete writing operation over I2C to multiple
       registers.
       Takes four inputs:
       - param device_address I2C address of the device to talk to.
       - param register_address Address of the first register to be written.
       - param register_count Number of registers that need to be written.
       - param data Array of data to be written
    */
    ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                                uint8_t register_address,
                                                uint8_t register_count,
                                                uint8_t* data);
    
    /* This function checks if a device is connected over the I2C lines.
       Takes one input:
       - param device_address I2C address of the device to be checked.
    
       Retval Returns true (>0) if device is connected.
    */
    uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address);
    
#endif // I2C_Interface_H

/* [] END OF FILE */
