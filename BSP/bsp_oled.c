#include "bsp_oled.h"
#include "i2c.h"
#include <string.h>

//数字字模
static const uint8_t Font5x7_Digit[10][5] =
{
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}  // 9
};
//大写英文字母
static const uint8_t Font5x7_Upper[26][5] =
{
    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x0C,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x7F,0x20,0x18,0x20,0x7F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x03,0x04,0x78,0x04,0x03}, // Y
    {0x61,0x51,0x49,0x45,0x43}  // Z
};
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
void OLED_Test(void)
{

    uint8_t page;
    uint8_t x;


    for(page=0;page<8;page++)
    {

        OLED_SetPos(0,page);


        for(x=0;x<128;x++)
        {
            OLED_SendData(0xff);
        }

    }

}
void OLED_PointTest(void)
{
    uint8_t x;
    uint8_t y;


    for(x=0;x<128;x++)
    {
        for(y=0;y<8;y++)
        {
            OLED_GRAM[x][y]=0xF0;
        }
    }


    OLED_Update();
}
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t state)
{
  if(x>=128||y>=64){
	  return;}
  uint8_t page=y/8;
  uint8_t bit=y%8;
  if(state)
  {
	  OLED_GRAM[x][page]|=(1<<bit);
  }
  else{
	  OLED_GRAM[x][page]&=~(1<<bit);
  }
}
void OLED_DrawLine(uint8_t x)//划竖线
{   uint8_t  i;//范围0-255
	if(x<128)
		{
	for(i=0;i<64;i++)
	{
	OLED_DrawPoint(x,i,1);
	}
}
	}
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
void OLED_ShowChar(uint8_t x, uint8_t y, char ch)
{
    const uint8_t *font = 0;

    uint8_t col;
    uint8_t row;

    static const uint8_t font_space[5] =
    {
        0x00,0x00,0x00,0x00,0x00
    };

    static const uint8_t font_colon[5] =
    {
        0x00,0x36,0x36,0x00,0x00
    };

    static const uint8_t font_minus[5] =
    {
        0x08,0x08,0x08,0x08,0x08
    };

    static const uint8_t font_dot[5] =
    {
        0x00,0x60,0x60,0x00,0x00
    };

    static const uint8_t font_slash[5] =
    {
        0x20,0x10,0x08,0x04,0x02
    };

    static const uint8_t font_question[5] =
    {
        0x02,0x01,0x51,0x09,0x06
    };


    /* 找到字符对应的字模 */
    if(ch >= '0' && ch <= '9')
    {
        font = Font5x7_Digit[ch - '0'];
    }
    else if(ch >= 'A' && ch <= 'Z')
    {
        font = Font5x7_Upper[ch - 'A'];
    }
    else
    {
        switch(ch)
        {
            case ' ':
                font = font_space;
                break;

            case ':':
                font = font_colon;
                break;

            case '-':
                font = font_minus;
                break;

            case '.':
                font = font_dot;
                break;

            case '/':
                font = font_slash;
                break;

            default:
                font = font_question;
                break;
        }
    }


    /* 5列 × 7行 */
    for(col = 0; col < 5; col++)
    {
        for(row = 0; row < 7; row++)
        {
            if(font[col] & (1 << row))
            {
                OLED_DrawPoint(x + col,
                               y + row,
                               1);
            }
            else
            {
                OLED_DrawPoint(x + col,
                               y + row,
                               0);
            }
        }
    }


    /* 最右边留1列空白，让两个字符不要粘起来 */
    for(row = 0; row < 7; row++)
    {
        OLED_DrawPoint(x + 5,
                       y + row,
                       0);
    }
}

void OLED_ShowString(uint8_t x,
                     uint8_t y,
                     const char *str)
{
    while(*str != '\0')
    {
        OLED_ShowChar(x, y, *str);

        x += 6;

        /* 一行放不下则换行 */
        if(x > 122)
        {
            x = 0;
            y += 8;
        }

        /* 超出屏幕 */
        if(y > 57)
        {
            break;
        }

        str++;
    }
}

void OLED_ShowNum(uint8_t x,
                  uint8_t y,
                  uint32_t num)
{
    char buffer[11];

    uint8_t index = 10;

    buffer[index] = '\0';


    do
    {
        index--;

        buffer[index] = (num % 10) + '0';

        num /= 10;

    } while(num > 0);


    OLED_ShowString(x,
                    y,
                    &buffer[index]);
}
	

