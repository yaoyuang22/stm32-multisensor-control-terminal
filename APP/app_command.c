#include "bsp_led.h"
#include "stm32f1xx_hal.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bsp_oled.h"
#include "bsp_sht30.h"
#include "bsp_servo.h"
#include "app_config.h"
#include "app_control.h"
#define CMD_SIZE 32
static char cmd[CMD_SIZE];

void Command_Process(char *cmd)
{  
	 
	 if(strcmp(cmd,"LED ON")==0)
		{
		  LED_ON();
		OLED_Clear();
		OLED_ShowString(0, 8, "UART: OK ");
		OLED_ShowString(0, 16, "LED: ON ");
        OLED_ShowString(0, 24, "CMD: LED ON");
        OLED_Update();
        printf("LED ON OK\r\n");
		}
	    else if(strcmp(cmd,"LED OFF")==0)
	    {
		  LED_OFF();
			OLED_Clear();
		OLED_ShowString(0, 8, "UART: OK ");
		OLED_ShowString(0, 16, "LED: OFF");
        OLED_ShowString(0, 24, "CMD: LED OFF");
        OLED_Update();
        printf("LED OFF OK\r\n");
		}
		 else if(strcmp(cmd,"READ")==0)
		 {
		  float temp;
          float hum;
        uint8_t status=SHT30_Read(&temp,&hum);
        if(status==0)
         { OLED_Clear();
         printf("temperature=%.2f\r\n",temp);
		OLED_ShowString(0, 8, "TEMP:");//一个字符5个像素+1个间隔 5个字符就是5×6=3
		OLED_ShowNum(30,8,(uint32_t)temp);
        printf("humidity=%.2f\r\n",hum);
		OLED_ShowString(0, 16, "HUM:");
		OLED_ShowNum(24,16,(uint32_t)hum);
            OLED_Update();
         }      
          else
          {OLED_Clear();
	      printf("SHT30 ERROR=%d\r\n", status);
	      }
    	}
		 else if(strcmp(cmd, "SAVE") == 0)
        {
           App_ConfigSave();
        }
        else if(strcmp(cmd, "LOAD") == 0)
        {
          App_ConfigLoad();
        }
		 else if(strncmp(cmd,"SERVO ",6)==0)
		 {
			 int angle;
			angle=atoi(&cmd[6]);
			 if((angle>=0)&&(angle<=180))
			 { 
				App_ControlSetServoAngle((uint8_t)angle);
				printf("SERVO OK angle=%d\r\n",angle);
				 
			 }
			 else
			 {
				 printf("SERVO RANGE ERROR\r\n");
			 }
		 }

	    else
	    { OLED_Clear();
		 OLED_ShowString(0, 24, "CMD: WRONG ");
         OLED_Update();
         printf("WRONG\r\n");
		}
      
}

void Command_Task(void)
{
    static uint8_t rx_data;//在函数内部用 static修饰局部变量，变量不再随函数调用创建销毁，而是在程序整个运行期间都存在​，且只在第一次执行到定义处时初始化一次因为在主函数循环时会反复调用防止重置
    static uint8_t idx = 0;
	if(RingBuffer_Read(&rx_data) == 0)
       {
		if(idx<sizeof(cmd)-1)
	    {
		if((rx_data=='\r')||(rx_data=='\n'))//单字节用‘’ 字符串用“”
		{
			if(idx>0)
			{	
			cmd[idx]='\0';
			Command_Process(cmd);
				idx=0;
			}
		}
		else
		 {
		 	cmd[idx]=rx_data;
			idx+=1;
			
		 }
	   
	     }
		else
		{
			idx=0;
		}
       } 
	
	
}

