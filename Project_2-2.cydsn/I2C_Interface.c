/* =============================================================================
 
    Electronic Technologies and Biosensors Laboratory
    Academic Year 2020/2021 - II Semester
    Final Project
    GROUP_02 - Variant 2

    I2C_Interface.c: source file

    This file includes all the required source code to interface the I2C 
    peripheral.
 
 * =============================================================================
*/

// Value returned if device present on I2C bus.
#ifndef DEVICE_CONNECTED
    #define DEVICE_CONNECTED 1
#endif

// Value returned if device not present on I2C bus.
#ifndef DEVICE_UNCONNECTED
    #define DEVICE_UNCONNECTED 0
#endif

#include "I2C_Interface.h" 
#include "I2C_Master.h"


ErrorCode I2C_Peripheral_Start(void) 
{
    // Start I2C peripheral
    I2C_Master_Start();  
    
    // Return no error since start function does not return any error
    return NO_ERROR;
}
    
    
ErrorCode I2C_Peripheral_Stop(void)
{
    // Stop I2C peripheral
    I2C_Master_Stop();
    // Return no error since stop function does not return any error
    return NO_ERROR;
}

ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                      uint8_t register_address,
                                      uint8_t* data)
{
    
    // Send start condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if( error == I2C_Master_MSTR_NO_ERROR ) {
        // Write address of register to be read
        error = I2C_Master_MasterWriteByte(register_address);
        if( error == I2C_Master_MSTR_NO_ERROR ) {
            // Send restart condition
            error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
            if( error == I2C_Master_MSTR_NO_ERROR ) {
                // Read data without acknowledgement
                *data = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
            }
        }
    }
    // Send stop condition
    I2C_Master_MasterSendStop();
    // Return error code
    return error ? I2C_READ_REGISTER_ERROR : NO_ERROR;    
    
}

ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                           uint8_t register_address,
                                           uint8_t register_count,
                                           uint8_t* data)
{
    
    // Send start condition
    uint8_t error = I2C_Master_MasterSendStart(device_address,I2C_Master_WRITE_XFER_MODE);
    if (error == I2C_Master_MSTR_NO_ERROR) {
        // Write address of register to be read with the MSb equal to 1
        register_address |= 0x80; // Datasheet indication for multi read -- autoincrement register address
        error = I2C_Master_MasterWriteByte(register_address);
        if (error == I2C_Master_MSTR_NO_ERROR) {
            // Send restart condition
            error = I2C_Master_MasterSendRestart(device_address, I2C_Master_READ_XFER_MODE);
            if (error == I2C_Master_MSTR_NO_ERROR) {
                // Continue reading until we have register to read
                uint8_t counter = register_count;
                while( counter > 1 ) {
                    data[register_count-counter] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
                    counter--;
                }
                // Read last byte and NACK to stop
                data[register_count-1] = I2C_Master_MasterReadByte(I2C_Master_NAK_DATA);
            }
        }
    }
    // Send stop condition
    I2C_Master_MasterSendStop();
    // Return error code
    return error ? I2C_READ_REGISTER_MULTI_ERROR : NO_ERROR;
    
}

ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                       uint8_t register_address,
                                       uint8_t data)
{
    
    // Start condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    if( error == I2C_Master_MSTR_NO_ERROR ) {
        // write address of register to overwrite
        error = I2C_Master_MasterWriteByte(register_address);
        if (error == I2C_Master_MSTR_NO_ERROR) {
            // write byte
            error = I2C_Master_MasterWriteByte(data);
        }
    }
    // Close I2C communication
    I2C_Master_MasterSendStop();
    
    // Return error code
    return error ? I2C_WRITE_REGISTER_ERROR : NO_ERROR;
    
}

ErrorCode I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data)
{
    
	// Send start condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    if( error == I2C_Master_MSTR_NO_ERROR ) {
        // Write address of register to be written with the MSB equal to 1
        register_address |= 0x80; // Datasheet indication for multi write -- autoincrement
        error = I2C_Master_MasterWriteByte(register_address);
        if( error == I2C_Master_MSTR_NO_ERROR ) {
            // Continue writing until we have data to write
            uint8_t counter = register_count;
            while( counter > 0 ) {
                error = I2C_Master_MasterWriteByte(data[register_count-counter]);
                if (error != I2C_Master_MSTR_NO_ERROR) {
                    // Send stop condition
                    I2C_Master_MasterSendStop();
                    // Return error code
                    return I2C_WRITE_REGISTER_ERROR;
                }
                counter--;
            }
        }
    }
    // Send stop condition in case something didn't work out correctly
    I2C_Master_MasterSendStop();
    // Return error code
    return error ? I2C_WRITE_REGISTER_MULTI_ERROR : NO_ERROR;
	
}


uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address)
{
    // Send a start condition followed by a stop condition
    uint8_t error = I2C_Master_MasterSendStart(device_address, I2C_Master_WRITE_XFER_MODE);
    I2C_Master_MasterSendStop();
    // If no error generated during stop, device is connected
    return (error == I2C_Master_MSTR_NO_ERROR ? DEVICE_CONNECTED : DEVICE_UNCONNECTED);
}

/* [] END OF FILE */
