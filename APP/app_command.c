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
#include "app_rtos.h"
#define CMD_SIZE 32
static char cmd[CMD_SIZE];

void Command_Process(char *cmd)
{  
	 
	 if(strcmp(cmd,"LED ON")==0)
		{
		  LED_ON();
		osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源）
        printf("LED ON OK\r\n");
		osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
		}
	    else if(strcmp(cmd,"LED OFF")==0)
	    {
		  LED_OFF();
		osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源）
        printf("LED OFF OK\r\n");
		osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
		}
		 else if(strcmp(cmd,"READ")==0)
		 {
		  float temp;
          float hum;
		osMutexAcquire(i2cMutexHandle, osWaitForever);//保护i2c
        uint8_t status=SHT30_Read(&temp,&hum);
		osMutexRelease(i2cMutexHandle);//命令进行完释放钥匙
        if(status==0)
         { 
		osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源）
         printf("temperature=%.2f\r\n",temp);
		 printf("humidity=%.2f\r\n",hum);
		osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
         }      
          else
          {
		  osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源）
	      printf("SHT30 ERROR=%d\r\n", status);
		  osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
	      }
    	}
		 else if(strcmp(cmd, "SAVE") == 0)
        {
         
			ControlMsg_t msg;
                msg.cmd   = CTRL_SAVE;
                msg.value = 0;
			if(osMessageQueuePut(controlQueueHandle,
                              &msg,
                              0,
                              0)!=osOK)
			{
				control_queue_drop_count++;
			}
        }
        else if(strcmp(cmd, "LOAD") == 0)
        {
      
			ControlMsg_t msg;
                msg.cmd   = CTRL_LOAD;
                msg.value = 0;
			if(osMessageQueuePut(controlQueueHandle,
                              &msg,
                              0,
                              0)!=osOK)
			{
				control_queue_drop_count++;
			}
        }
		 else if(strncmp(cmd,"SERVO ",6)==0)
		 {
			 int angle;
			angle=atoi(&cmd[6]);
			 if((angle>=0)&&(angle<=180))
			 { 
				
				ControlMsg_t msg;
                msg.cmd   = CTRL_SERVO_SET;
                msg.value = angle;
                if(osMessageQueuePut(controlQueueHandle,
                              &msg,
                              0,
                              0)!=osOK)
			{
				control_queue_drop_count++;
			}
				 osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源
				 printf("SERVO OK angle=%d\r\n",angle);
				 osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
			 }
			 else
			 {  osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源
				 printf("SERVO RANGE ERROR\r\n");
				 osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
			 }
		 }

	    else
	    { 
		osMutexAcquire(uartMutexHandle, osWaitForever);//多task同时访问同一个资源时作为钥匙，谁拿到谁才能用（保护资源
         printf("WRONG\r\n");
		osMutexRelease(uartMutexHandle);//命令进行完释放钥匙
		}
      
}

void Command_Task(void)
{
    uint8_t rx_data;
    static uint8_t idx = 0;//在函数内部用 static修饰局部变量，变量不再随函数调用创建销毁，而是在程序整个运行期间都存在​，且只在第一次执行到定义处时初始化一次因为在主函数循环时会反复调用防止重置
	while (RingBuffer_Read(&rx_data) == 0)
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

