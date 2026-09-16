#include "usrt.h"
#include "stm32f1xx_hal.h"

void uart_transmit(uint8_t *data1,uint16_t length)
{
	HAL_UART_Transmit(&huart1,data1,length,1000)​​
}

void uart_receive(uint8_t *data1,uint16_t length)
{
	HAL_UART_Receive(&huart1,data1,length,1000)
}


