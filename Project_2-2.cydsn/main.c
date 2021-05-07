/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "I2C_Interface.h"
#include "LIS3DH.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_Peripheral_Start();
    UART_DEBUG_Start();
    
    CyDelay(5); //"The boot procedure is complete about 5 ms after device power-up."
    
    
    char message[50] = {'\0'};
    
    UART_DEBUG_PutString("\f**************\r\n");
    UART_DEBUG_PutString("** I2C Scan **\r\n");
    UART_DEBUG_PutString("**************\r\n");

    CyDelay(10);
    
    uint32_t rval;
    
    // Setup the screen and print the header
	UART_DEBUG_PutString("\n\n   ");
	for(uint8_t i = 0; i<0x10; i++)
	{
        sprintf(message, "%02X ", i);
		UART_DEBUG_PutString(message);
	}
    
    // SCAN the I2C BUS for slaves
	for( uint8_t i2caddress = 0; i2caddress < 0x80; i2caddress++ ) {
        
		if(i2caddress % 0x10 == 0 ) {
            sprintf(message, "\n%02X ", i2caddress);
		    UART_DEBUG_PutString(message);
        }
 
		rval = I2C_Master_MasterSendStart(i2caddress, I2C_Master_WRITE_XFER_MODE);
        
        if( rval == I2C_Master_MSTR_NO_ERROR ) // If you get ACK then print the address
		{
            sprintf(message, "%02X ", i2caddress);
		    UART_DEBUG_PutString(message);
		}
		else //  Otherwise print a --
		{
		    UART_DEBUG_PutString("-- ");
		}
        I2C_Master_MasterSendStop();
	}
	UART_DEBUG_PutString("\n\n");
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    // Your turn to code!
    // Check the defines in LIS3DH.h for the address of the
    // registers to read
    
    /*      I2C Master Read - WHOAMI Register       */
    uint8_t whoami_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  LIS3DH_WHO_AM_I_REG_ADDR,
                                                  &whoami_reg);
    if( error == NO_ERROR ) {
        sprintf(message, "WHOAMI reg value: 0x%02X [Expected value: 0x%02X]\r\n", whoami_reg, LIS3DH_WHOAMI_RETVAL);
        UART_DEBUG_PutString(message);
    }
    else {
        UART_DEBUG_PutString("I2C error while reading LIS3DH_WHO_AM_I_REG_ADDR\r\n");
    }
    
    /*      I2C Master Read - STATUS Register       */
    uint8_t status_reg;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  LIS3DH_REG_OUT_X_L,
                                                  &status_reg);
    if( error == NO_ERROR ) {
        sprintf(message, "LIS3DH_STATUS_REG value: 0x%02X\r\n", status_reg);
        UART_DEBUG_PutString(message);
    }
    else {
        UART_DEBUG_PutString("I2C error while reading LIS3DH_STATUS_REG\r\n");
    }
    
    
    //enable data rate
    uint8_t ctrl_reg1;
    ctrl_reg1 = 0b01110111;
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  LIS3DH_CTRL_REG1,
                                                  ctrl_reg1);
    if( error == NO_ERROR ) {
        sprintf(message, "CTRL_REG1 successfully written as: 0x%02X\r\n", ctrl_reg1);
        UART_DEBUG_PutString(message);
    }
    else {
        UART_DEBUG_PutString("I2C error while writing CTRL_REG1\r\n");
    }
   
    
    for(;;)
    {
        CyDelay(10);
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  LIS3DH_REG_OUT_X_H,
                                                  &status_reg);
        if( error == NO_ERROR ) {
            sprintf(message, "LIS3DH_STATUS_REG value: 0x%02X\r\n", status_reg);
            UART_DEBUG_PutString(message);
        }
        else {
            UART_DEBUG_PutString("I2C error while reading LIS3DH_STATUS_REG\r\n");
        }
    }
}

/* [] END OF FILE */
