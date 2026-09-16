#include "app_monitor.h"
#include "bsp_sht30.h"
#include "bsp_oled.h"
#include "app_control.h"
#include <stdio.h>

static volatile uint8_t monitor_flag = 0;

static uint16_t ADC_GetAverage(uint16_t *data,uint8_t length)//app_monitor.c 内部为了完成自己的任务而使用的小工具，外面的 main.c、app_command.c 都没必要调用它。所以只是static
	
{   uint8_t i;
	uint32_t sum=0;//如果uint16_t 最多只能进行16bit数组之和，防止数组长度超过16
	if(length==0)
	{
		return 0;
	}
	for(i=0;i<length;i++) 
	{
	 sum+=data[i];
     }
	return (uint16_t)(sum/length);
	
}
void App_MonitorTask(uint16_t *adc_buffer, uint8_t length)
{
	if(monitor_flag == 0)
    {
        return;//这1秒还没到？→ 什么都不做→ 马上退出函数
    }
	uint8_t angle;
    monitor_flag = 0;	
	uint32_t adc_raw;//因为它们只服务于：“执行这一次1秒监测任务” 所以不需要在main里长期存在也就不需要static
    float voltage;
    float temp;
    float hum;
    uint8_t status;
	angle = App_ControlGetServoAngle();
	adc_raw = ADC_GetAverage(adc_buffer, length);//取得ADC原始数据的平均值
         voltage = adc_raw * 3.3f / 4095.0f;
		   status=SHT30_Read(&temp,&hum);
		  if(status==0)  
         {	
		 printf("ADC_RAW=%lu Voltage=%.2fV\r\n",
			    adc_raw,voltage);
         printf("temperature=%.2f\r\n",temp);
		 printf("humidity=%.2f\r\n",hum);
		//一个字符5个像素+1个间隔 5个字符就是5×6=3
			 OLED_Clear();
		OLED_ShowString(0, 0, "ADC:");
        OLED_ShowNum(24, 0, adc_raw);

        OLED_ShowString(0, 8, "TEMP:");
        OLED_ShowNum(30, 8, (uint32_t)temp);

        OLED_ShowString(0, 16, "HUM:");
        OLED_ShowNum(24, 16, (uint32_t)hum);
		
		OLED_ShowString(0, 24, "ANGEL:");
        OLED_ShowNum(36, 24, (uint32_t)angle);
	      OLED_Update();
         }      
          else
          {
	      printf("SHT30 ERROR=%d\r\n", status);
	      }
}
void App_MonitorTick(void)
{
	monitor_flag = 1;
}
