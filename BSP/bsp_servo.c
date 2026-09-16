#include "bsp_servo.h"
#include "tim.h"

static uint8_t current_angle = 90;//当前角度为多少？以便外部文件调用

uint8_t Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;
    uint32_t test;

    if(angle > 180)
    {
        return 1;
    }

    test = angle * 1000;//32bit存可能超过16bit的数据
    pulse = 1000 + test / 180;//算出脉冲数值范围在1000-2000 1000代表0度，2000代表180度

    __HAL_TIM_SET_COMPARE(&htim2,   //根据角度算出的脉冲值改变CCR的数值从而改变占空比 
                          TIM_CHANNEL_2,
                          pulse);

    return 0;
}

uint8_t Servo_GetAngle(void)
{
    return current_angle;

}
