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

#include "LIS3DH.h"


ErrorCode set_reg (uint8_t reg, uint8_t value_reg)
{
    ErrorCode error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS, 
                                                   reg,
                                                   value_reg);
    return error;
}

ErrorCode get_reg (uint8_t reg, uint8_t* value_reg)
{
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS, 
                                                  reg,
                                                  value_reg);
    return error;
}

/* [] END OF FILE */
