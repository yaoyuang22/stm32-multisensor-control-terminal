#ifndef __APP_MONITOR_H
#define __APP_MONITOR_H

#include <stdint.h>
void App_MonitorTask(void);//把ADC DMA 缓冲区传进去这样，app_monitor.c 不需要知道 adc_buffer 在 main 里到底怎么定义。

#endif
