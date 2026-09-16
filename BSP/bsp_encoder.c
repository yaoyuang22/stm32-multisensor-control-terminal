#include "bsp_encoder.h"
#include "main.h"
static volatile int32_t encoder_count = 0;//int是因为旋转编码器有正负  static让其本来在main中谁都能修改改成	内部自己管理 encoder_count外界想知道数值必须调用 Encoder_GetCount()
static volatile uint8_t encoder_changed = 0; 
int32_t Encoder_GetCount(void)
{
    return encoder_count;
}

uint8_t Encoder_IsChanged(void)
{
    return encoder_changed;
}

void Encoder_ClearChanged(void)
{
    encoder_changed = 0;
}

void Encoder_Reset(void)
{
    encoder_count = 0;
}
void Encoder_EXTI_Handler(void)
{
    static uint32_t last_encoder_tick = 0;
    uint32_t now = HAL_GetTick();//取STM32到现在为止的时间ms数

    if(now - last_encoder_tick >= 3)
    {
        last_encoder_tick = now;

        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET)
        {
            encoder_count++;
        }
        else
        {
            encoder_count--;
        }

        encoder_changed = 1;
    }
}

