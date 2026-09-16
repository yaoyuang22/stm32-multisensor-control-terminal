#include "bsp_servo.h"
#include "tim.h"

uint8_t Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;
    uint32_t test;

    if(angle > 180)
    {
        return 1;
    }

    test = angle * 1000;
    pulse = 1000 + test / 180;

    __HAL_TIM_SET_COMPARE(&htim2,
                          TIM_CHANNEL_2,
                          pulse);

    return 0;
}