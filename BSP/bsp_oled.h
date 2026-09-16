#ifndef __BSP_OLED_H
#define __BSP_OLED_H


#include "main.h"
#include "stdint.h"


/*
 * OLED I2C地址
 *
 * OLED实际7bit地址:
 * 0x3C
 *
 * HAL库使用8bit地址:
 * 0x3C << 1 = 0x78
 */
#define OLED_ADDR 0x78


/*
 * OLED初始化
 *
 * 包括:
 * 1. 初始化SSD1306寄存器
 * 2. 打开显示
 */
void OLED_Init(void);
void OLED_PointTest(void);
void OLED_Test(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t state);
void OLED_DrawLine(uint8_t x);
void OLED_ShowChar(uint8_t x, uint8_t y, char ch);
void OLED_ShowString(uint8_t x, uint8_t y, const char *str);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num);
/*
 * 清空显存
 *
 * 注意：
 * 清缓存，不一定立即刷新屏幕
 *
 * 需要调用:
 * OLED_Update()
 *
 */
void OLED_Clear(void);



/*
 * 刷新OLED
 *
 * 将STM32内部显存
 *
 * 发送到OLED
 */
void OLED_Update(void);



/*
 * 设置显示位置

 x:
 0~127

 y:
 0~7

 OLED12864:
 一共有8页

 */
void OLED_SetPos(uint8_t x,uint8_t y);



/*
 * 显示一个字符

 x:
 横坐标

 y:
 页坐标

 chr:
 字符

 */


#endif
