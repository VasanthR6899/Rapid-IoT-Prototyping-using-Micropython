#ifndef _SAME54_PY_I2C_

#define _SAME54_PY_I2C_

#endif

#include "definitions.h"

#define I2C_NUM_MAX 2
#define I2C_NUM_1 1
#define I2C_NUM_2 2

#define I2C1_SCL_PIN 93
#define I2C1_SDA_PIN 92

#define I2C2_SCL_PIN 48
#define I2C2_SDA_PIN 47

#define APP_TRANSFER_STATUS_IN_PROGRESS (0)
#define APP_TRANSFER_STATUS_SUCCESS (1)
#define APP_TRANSFER_STATUS_ERROR (2)

bool I2C_TransferSetup(uint8_t i2c_num,SERCOM_I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq);

bool I2C_Read(uint8_t i2c_num, uint16_t address, uint8_t* rdData, uint32_t rdLength);

bool I2C_Write(uint8_t i2c_num, uint16_t address, uint8_t* wrData, uint32_t wrLength);

void I2C_CallbackRegister(uint8_t i2c_num);

