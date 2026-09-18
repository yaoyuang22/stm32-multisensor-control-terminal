#include "bsp_adc.h"
#include "adc.h"

#define ADC_BUF_SIZE 16

static uint16_t adc_buffer[ADC_BUF_SIZE];//ADC是0~4095 也就是2的12次方 uint8_t装不下 16的话就0~65535 对应HALF WORD 半字=2byte=16bit	

void BSP_ADC_Start(void)
{
HAL_ADCEx_Calibration_Start(&hadc1);//开启ADC转换之前先进行ADC校准
HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_buffer,ADC_BUF_SIZE);//启动 ADC1，并使用它所关联的 DMA，把 ADC 转换结果连续存入 adc_buffer，一轮长度为 ADC_BUF_SIZE。DMA和usart1 rx的channel不一样
}

uint16_t BSP_ADC_GetAverage(void)//app_monitor.c 不只是内部为了完成自己的任务而使用的小工具，外面的 main.c、app_command.c 有必要调用它。所以不能是static
	
{   uint8_t i;
	uint32_t sum=0;//如果uint16_t 最多只能进行16bit数组之和，防止数组长度超过16
	if(ADC_BUF_SIZE ==0)
	{
		return 0;
	}
	for(i=0;i<ADC_BUF_SIZE ;i++) 
	{
	 sum+=adc_buffer[i];
     }
	return (uint16_t)(sum/ADC_BUF_SIZE);
	
}