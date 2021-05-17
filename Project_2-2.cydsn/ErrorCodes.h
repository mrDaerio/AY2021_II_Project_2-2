/**
*   \brief Error codes.
* 
*   This definition defines several error codes that will
*   be used throughout the project.
*/
#ifndef __ERRORCODES_H
    #define __ERRORCODES_H
    
    typedef enum {
        NO_ERROR,           ///< No error generated
        I2C_READ_REGISTER_ERROR,  ///< Error in reading I2C register 
        I2C_WRITE_REGISTER_ERROR,   ///< Error in writing I2C register 
        I2C_READ_REGISTER_MULTI_ERROR,  ///< Error in reading multiple I2C registers 
        I2C_WRITE_REGISTER_MULTI_ERROR,  ///< Error in writing multiple I2C registers
        DATARATE_SET_FAIL,  ///<error in setting data rate, check set_datarate function
        FSR_SET_FAIL,  ///<error in setting fsr, check set_fsr function
        FIFO_SET_FAIL,  ///<error in setting FIFO, check FIFO_set function
    } ErrorCode;
    
    void error_check (ErrorCode error);

#endif
/* [] END OF FILE */
