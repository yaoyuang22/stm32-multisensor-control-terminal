#include "bsp_led.h"
#include "stm32f1xx_hal.h"
#include "uart.h"
#include <string.h>
static uint8_t data_save[32];
static uint8_t idx=0;

void Command_Translate(uint8_t *data_in)
{  if(idx>sizeof(data_save)-2)
	{
		printf("too long\r\n");
		idx=0;
	}
	else
	{

	if(*data_in=='\r'||*data_in=='\n')
	   {
		data_save[idx]='\0';
		if(idx==0)
		{
			return;
		}
		if(strcmp((char*)data_save,"LED ON")==0)
		{
		  LED_ON();
		}
	    else if(strcmp((char*)data_save,"LED OFF")==0)
	    {
		  LED_OFF();
	    }
	    else
	    { 
		  printf("WRONG\r\n");
		}
		idx=0;
	   }  
	else
	{
      data_save[idx]=*data_in;
	  idx++;
	}
  }	
}