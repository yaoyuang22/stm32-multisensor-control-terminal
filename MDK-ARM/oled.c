#include "bsp_oled.h"
#include "i2c.h"
#include <string.h>


/*
    OLED显存

    OLED:
    128×64

    一页:
    128×8

    共8页

    OLED_GRAM[x][page]

*/
static uint8_t OLED_GRAM[128][8];



/*
    I2C句柄

    CubeMX生成:

    I2C1
    hi2c1

*/
extern I2C_HandleTypeDef hi2c1;



/*
    向OLED发送一个命令

    SSD1306规定:

    0x00 + command

*/
static void OLED_SendCommand(uint8_t command)
{

    uint8_t data[2];


    data[0] = 0x00;

    data[1] = command;



    HAL_I2C_Master_Transmit(
        &hi2c1,
        OLED_ADDR,
        data,
        2,
        100
    );

}



/*
    向OLED发送一个数据

    SSD1306规定:

    0x40 + data

*/
static void OLED_SendData(uint8_t data)
{

    uint8_t buf[2];


    buf[0] = 0x40;

    buf[1] = data;



    HAL_I2C_Master_Transmit(
        &hi2c1,
        OLED_ADDR,
        buf,
        2,
        100
    );

}



/*
    设置OLED显示位置

    x:
    0~127

    y:
    0~7

*/
void OLED_SetPos(uint8_t x,uint8_t y)
{

    OLED_SendCommand(
        0xB0 + y
    );


    OLED_SendCommand(
        ((x & 0xF0)>>4) | 0x10
    );


    OLED_SendCommand(
        x & 0x0F
    );

}



/*
    SSD1306初始化

*/
void OLED_Init(void)
{

    HAL_Delay(100);



    OLED_SendCommand(0xAE); //关闭显示



    OLED_SendCommand(0x20);
    OLED_SendCommand(0x10);



    OLED_SendCommand(0xB0);



    OLED_SendCommand(0xC8);



    OLED_SendCommand(0x00);



    OLED_SendCommand(0x10);



    OLED_SendCommand(0x40);



    OLED_SendCommand(0x81);
    OLED_SendCommand(0xFF);



    OLED_SendCommand(0xA1);



    OLED_SendCommand(0xA6);



    OLED_SendCommand(0xA8);
    OLED_SendCommand(0x3F);



    OLED_SendCommand(0xD3);
    OLED_SendCommand(0x00);



    OLED_SendCommand(0xD5);
    OLED_SendCommand(0x80);



    OLED_SendCommand(0xD9);
    OLED_SendCommand(0xF1);



    OLED_SendCommand(0xDA);
    OLED_SendCommand(0x12);



    OLED_SendCommand(0xDB);
    OLED_SendCommand(0x40);



    OLED_SendCommand(0x8D);
    OLED_SendCommand(0x14);



    OLED_SendCommand(0xAF); //开启显示



    OLED_Clear();

    OLED_Update();

}



/*
    清空缓存

*/
void OLED_Clear(void)
{

    memset(
        OLED_GRAM,
        0,
        sizeof(OLED_GRAM)
    );

}



/*
    刷新OLED

    把:

    STM32 RAM

    OLED_GRAM

    发送到OLED

*/

void OLED_Update(void)
{

    uint8_t page;


    uint8_t x;



    for(page=0;page<8;page++)
    {


        OLED_SetPos(
            0,
            page
        );


        for(x=0;x<128;x++)
        {

            OLED_SendData(
                OLED_GRAM[x][page]
            );

        }


    }


}