#include "definitions.h"


size_t UART_ReadCountGet()
{
    return SERCOM5_USART_ReadCountGet();
}
        
size_t UART_Read(uint8_t* pRdBuffer, const size_t size)
{
    return SERCOM5_USART_Read(pRdBuffer, size);
}

size_t UART_Write(uint8_t* pWrBuffer, const size_t size )
{
    return SERCOM5_USART_Write(pWrBuffer, size );
}

bool UART_SerialSetup(USART_SERIAL_SETUP * serialSetup, uint32_t clkFrequency)
{
    return SERCOM5_USART_SerialSetup(serialSetup, clkFrequency );
}

