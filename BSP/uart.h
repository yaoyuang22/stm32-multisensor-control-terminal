#ifndef __UART_H
#define __UART_H
#include <stdint.h>
#include "stdio.h"

void uart_transmit(uint8_t *data1,uint16_t length);
void uart_receive(uint8_t *data1,uint16_t length);
int fputc(int ch,FILE *f);
uint8_t RingBuffer_Write(uint8_t data);
uint8_t RingBuffer_Read(uint8_t *data);
extern volatile uint32_t ring_overflow_count;
#endif
