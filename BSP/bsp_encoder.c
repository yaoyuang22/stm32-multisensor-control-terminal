#include "bsp_encoder.h"
#include "main.h"

int8_t Encoder_EXTI_Handler(void)
{
    static uint32_t last_encoder_tick = 0;
    uint32_t now = HAL_GetTick();//取STM32到现在为止的时间ms数
	if(now - last_encoder_tick < 3)
	{
		return 0;
	}
 
        last_encoder_tick = now;
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET)
        {
			return 1;
        }
        else
        {
			return -1;
        }

    
}

