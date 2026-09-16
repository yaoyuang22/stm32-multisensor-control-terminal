#include "uart.h"
#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart1;
#include "stdio.h"
#define UART_RING_SIZE 64
static uint8_t uart_ring[UART_RING_SIZE];
static volatile uint8_t write_index = 0;
static volatile uint8_t read_index = 0;
static volatile uint32_t ring_overflow_count = 0;
void uart_transmit(uint8_t *data1,uint16_t length)
{
	HAL_UART_Transmit(&huart1,data1,length,1000);
}

void uart_receive(uint8_t *data1,uint16_t length)
{
	HAL_UART_Receive(&huart1,data1,length,5000);
}


int fputc(int ch,FILE *f)
{
	uint8_t temp=(uint8_t)ch;
	HAL_UART_Transmit(&huart1,&temp,1,1000);
	return ch;
}

uint8_t  RingBuffer_Write(uint8_t data)
{  uint8_t next_write_index=(write_index+1)%UART_RING_SIZE;
	if(next_write_index==read_index)
	{
		ring_overflow_count++;
		return 2;//满
	}
	uart_ring[write_index]=data;
	write_index=next_write_index;
	
	return 0;
}

uint8_t RingBuffer_Read(uint8_t *data)
{   if(read_index==write_index)
	{return 1;}//空
    *data=uart_ring[read_index];
	read_index=(read_index+1)%UART_RING_SIZE;
	return 0;
}


