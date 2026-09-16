#include "app_config.h"
#include "app_control.h"
#include "bsp_w25q64.h"
#include <stdio.h>
#define APP_CONFIG_ADDR    0x001000U
typedef struct
{
    uint8_t magic1;
    uint8_t magic2;
    uint8_t version;
    uint8_t servo_angle;
} AppConfig_t;  //定义结构体变量名AppConfig_t

void App_ConfigSave(void) //保存数据到flash内部
{
    AppConfig_t config;

    config.magic1 = 0xA5;//
    config.magic2 = 0x5A;
    config.version = 1;
    config.servo_angle = App_ControlGetServoAngle();

    W25Q64_SectorErase(APP_CONFIG_ADDR);

    W25Q64_PageProgram(APP_CONFIG_ADDR,
                       (uint8_t *)&config,
                       sizeof(config));

    printf("SAVE OK, ANGLE = %d\r\n",
           config.servo_angle);
}