#include "bsp_led.h"
#include "stm32f1xx_hal.h"

void LED_ON(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
}

void LED_OFF(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
}

void LED_Turn(void)
{
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}
