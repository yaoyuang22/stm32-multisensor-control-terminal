#include "app_control.h"
#include "bsp_encoder.h"
#include "bsp_servo.h"
#include <stdio.h>
#include <stdint.h>

static int16_t servo_angle = 90;
void App_ControlSetServoAngle(uint8_t angle)//使外部能控制sg90的角度并且将其保存到servo_angle内部，这样以后整个工程里改变舵机目标角度，都尽量走该函数保持一致而不是调用Servo_SetAngle
{
    if(angle > 180)
    {
        angle = 180;
    }

    servo_angle = angle;
    
    Servo_SetAngle(angle);
}
uint8_t App_ControlGetServoAngle(void)//使外部能够读取servo_angle此时的角度
{
    return (uint8_t)servo_angle;
}

void App_ControlInit(void) //控制初始化
{
    servo_angle = 90;

    Servo_SetAngle((uint8_t)servo_angle);
}

void App_ControlEncoderStep(int32_t step)  //控制角度
{

    servo_angle += step * 5;//角度+-差值的5倍，并且明确servo_angle为角度值不需要知道 Encoder_GetCount()的值

    if(servo_angle > 180)//限制角度范围0-180
    {
        servo_angle = 180;
    }

    if(servo_angle < 0)
    {
        servo_angle = 0;
    }

    App_ControlSetServoAngle((uint8_t)servo_angle);

    printf("SERVO ANGLE = %d\r\n", servo_angle);
 
}
