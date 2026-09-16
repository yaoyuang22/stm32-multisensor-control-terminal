#include "bsp_oled.h"
#include "i2c.h"
#include <string.h>
#include "uart.h"
#include "bsp_oled.h"

uint8_t SHT30_CRC8(uint8_t *data,uint8_t len)
{
	uint8_t  crc=0xff;
	uint8_t i;
	uint8_t j;
	for(i=0;i<len;i++)
	{
		crc^=data[i];//检查每一个Byte
		for(j=0;j<8;j++)
		{
			if(crc&0x80)
			{
				crc=(crc<<1)^0x31;
			}
			else
			{
				crc<<=1;
			}
		}
		
	}
	return crc;
	
}
uint8_t SHT30_Read(float *temperature,float *humidity)
{   uint8_t cmd[2]={0x24,0x00};
	if(HAL_I2C_Master_Transmit(&hi2c1,0x44<<1,cmd,2,500)==HAL_OK)
	{
	HAL_Delay(20);
	uint8_t data[6];
	if(HAL_I2C_Master_Receive(&hi2c1,0x44<<1,data,6,500)==HAL_OK)
	{
	//printf("%02X %02X %02X %02X %02X %02X\r\n",
      // data[0],
      // data[1],
      //data[2],
      //data[3],
     //data[4],
    // data[5]);
  uint16_t raw_temp=((uint16_t) data[0]<<8)|(data[1]);
  uint16_t raw_humi=((uint16_t) data[3]<<8)|(data[4]);
		
		if(SHT30_CRC8(data,2)==data[2]);
		//{printf("raw_temp=%u\r\n", raw_temp);}
		else{return 3;}
		if(SHT30_CRC8(&data[3],2)==data[5]);
		//{printf("raw_humi=%u\r\n", raw_humi);}
		else{return 4;}
  *temperature =-45.0f +175.0f * raw_temp / 65535.0f;
   *humidity =100.0f * raw_humi / 65535.0f;
 // printf("temperature=%.2f\r\n",*temperature);
		//OLED_ShowString(0, 8, "TEMP:");//一个字符5个像素+1个间隔 5个字符就是5×6=3
		//OLED_ShowNum(30,8,(uint32_t)temperature);
 //  printf("humidity=%.2f\r\n",*humidity);
		//OLED_ShowString(0, 16, "HUM:");
		//OLED_ShowNum(24,16,(uint32_t)humidity);
		return 0;
	}
	else{
		return 1;}
	}
	else
	{return 2;}
}
