#include "definitions.h"
#include "same54p20a_i2c.h"


extern uint8_t transferStatus;

void APP_I2C1Callback(uintptr_t context )
{
    
    if(SERCOM3_I2C_ErrorGet() == SERCOM_I2C_ERROR_NONE)
    {
        transferStatus = APP_TRANSFER_STATUS_SUCCESS;        
    }
    else
    {
        transferStatus = APP_TRANSFER_STATUS_ERROR;
        
    }
}

void APP_I2C2Callback(uintptr_t context )
{
    
    if(SERCOM7_I2C_ErrorGet() == SERCOM_I2C_ERROR_NONE)
    {
        transferStatus = APP_TRANSFER_STATUS_SUCCESS;        
    }
    else
    {
        transferStatus = APP_TRANSFER_STATUS_ERROR;
        
    }
}

bool I2C_TransferSetup(uint8_t i2c_num,SERCOM_I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
 {
    if (i2c_num == I2C_NUM_1) 
    {
        SERCOM3_I2C_TransferSetup(&setup, 100000000UL);
    } 
    else if (i2c_num == I2C_NUM_2) 
    {
        SERCOM7_I2C_TransferSetup(&setup, 100000000UL);
    }
 }
    
bool I2C_Read(uint8_t i2c_num, uint16_t address, uint8_t* rdData, uint32_t rdLength)
{
    bool result;
    if(i2c_num == I2C_NUM_1)
    {
        while (SERCOM3_I2C_IsBusy());
        result = SERCOM3_I2C_Read(address, rdData, rdLength); 
        return result;
    }
    else if(i2c_num == I2C_NUM_2)
    {
        while (SERCOM7_I2C_IsBusy());
        result = SERCOM7_I2C_Read(address, rdData, rdLength);    
        return result;
    }
}

bool I2C_Write(uint8_t i2c_num, uint16_t address, uint8_t* wrData, uint32_t wrLength)
{
    bool result;
    if(i2c_num == I2C_NUM_1)
    {
        while (SERCOM3_I2C_IsBusy());
        result = SERCOM3_I2C_Write(address, wrData, wrLength); 
        return result;
    }
    else if(i2c_num == I2C_NUM_2)
    {
        while (SERCOM7_I2C_IsBusy());
        result = SERCOM7_I2C_Write(address, wrData, wrLength);
        return result;        
    }
}

void I2C_CallbackRegister(uint8_t i2c_num)
{
    if (i2c_num == I2C_NUM_1) 
    {
        SERCOM3_I2C_CallbackRegister( APP_I2C1Callback, NULL );
    } 
    else if (i2c_num == I2C_NUM_2) 
    {
        SERCOM7_I2C_CallbackRegister( APP_I2C2Callback, NULL );
    }
}