#ifndef __APP_MONITOR_H
#define __APP_MONITOR_H

#include <stdint.h>
void App_MonitorTick(void);
void App_MonitorTask(uint16_t *adc_buffer, uint8_t length);//把ADC DMA 缓冲区传进去这样，app_monitor.c 不需要知道 adc_buffer 在 main 里到底怎么定义。

#endif
