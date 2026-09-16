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

    config.magic1 = 0xA5;//给保存定制身份证前俩位
    config.magic2 = 0x5A;
    config.version = 1;
    config.servo_angle = App_ControlGetServoAngle();//保存的角度

    W25Q64_SectorErase(APP_CONFIG_ADDR);//先擦除扇区

    W25Q64_PageProgram(APP_CONFIG_ADDR, //进行页编程写入
                       (uint8_t *)&config,
                       sizeof(config));

    printf("SAVE OK, ANGLE = %d\r\n",
           config.servo_angle);
}
uint8_t App_ConfigLoad(void)//读取w25q64中的数据
{
    AppConfig_t config;//生成结构体变量用于存数据

    W25Q64_ReadData(APP_CONFIG_ADDR,
                    (uint8_t *)&config,
                    sizeof(config));

    printf("LOAD RAW: %02X %02X %02X %02X\r\n",
           config.magic1,
           config.magic2,
           config.version,
           config.servo_angle);

    if((config.magic1 != 0xA5) ||
       (config.magic2 != 0x5A))
    {
        printf("LOAD FAIL: INVALID MAGIC\r\n");
        return 1;
    }

    if(config.version != 1)
    {
        printf("LOAD FAIL: INVALID VERSION\r\n");
        return 2;
    }

    if(config.servo_angle > 180)
    {
        printf("LOAD FAIL: INVALID ANGLE\r\n");
        return 3;
    }

    App_ControlSetServoAngle(config.servo_angle);//将存到的角度读取到现在的角度

    printf("LOAD OK, ANGLE = %d\r\n",
           config.servo_angle);

    return 0;
}
