#include "bsp_key.h"
#include "main.h"

static volatile uint8_t key_press_event = 0; //按压事件是否发生，1是发生，0是没发生
static volatile uint8_t key_locked = 0;//是否上锁，0是没有，1是锁住

static uint8_t release_checking = 0;
static uint32_t release_tick = 0;



void Key_EXTI_Handler(void)
{
    static uint32_t last_key_tick = 0;
    uint32_t now = HAL_GetTick();
    if((now - last_key_tick >= 20) &&
       (key_locked == 0))
    {
        last_key_tick = now;

        key_press_event = 1;
        key_locked = 1;
    }
}

void Key_Task(void)
{
if(key_locked) //按键的锁住函数
{
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
    {
        /* 第一次检测到松开 */
        if(release_checking == 0)
        {
            release_checking = 1;
            release_tick = HAL_GetTick();
        }
        else
        {
            /* 已经连续保持高电平50ms */
            if(HAL_GetTick() - release_tick >= 50)
            {
                key_locked = 0;
                release_checking = 0;
            }
        }
    }
    else
    {
        /* 又变成低电平，说明刚才只是松开抖动 */
        release_checking = 0;
    }
}
}

uint8_t Key_GetPressEvent(void)
{
    if(key_press_event)
    {
        key_press_event = 0;
        return 1;
    }

    return 0;
}

