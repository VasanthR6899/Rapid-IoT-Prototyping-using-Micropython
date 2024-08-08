#ifndef _SAME54_PY_UART_

#define _SAME54_PY_UART_

#endif

#include "definitions.h"


#define UART_2_TX_PIN 82
#define UART_2_RX_PIN 83

size_t UART_ReadCountGet();

size_t UART_Write(uint8_t* pWrBuffer, const size_t size );

size_t UART_Read(uint8_t* pRdBuffer, const size_t size);

bool UART_SerialSetup(USART_SERIAL_SETUP * serialSetup, uint32_t clkFrequency);