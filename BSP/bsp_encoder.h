#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H

#include <stdint.h>

void Encoder_EXTI_Handler(void);//编码器发生外部中断时，让BSP自己处理

int32_t Encoder_GetCount(void);//获取当前位置

uint8_t Encoder_IsChanged(void);//有没有发生旋转

void Encoder_ClearChanged(void);//清除变化标志

void Encoder_Reset(void);//计数清零

#endif
