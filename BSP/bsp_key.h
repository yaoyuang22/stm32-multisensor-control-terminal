#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include <stdint.h>
void Key_EXTI_Handler(void);//PB1下降沿来了，处理中断这一部分

void Key_Task(void);//每轮main调用，用来处理“松开稳定50ms”之类的状态

uint8_t Key_GetPressEvent(void);//问按键模块：“有没有一次新的有效按下？”

#endif

