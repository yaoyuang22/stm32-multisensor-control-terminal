#include "bsp_w25q64.h"
#include "spi.h"
#include "gpio.h"

uint8_t W25Q64_WriteEnable(void)
{   uint8_t tx=0x06;
	HAL_StatusTypeDef status;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//拉低cs选中w25q64
	status=HAL_SPI_Transmit(&hspi1,&tx,1,100);//发送写指令
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//拉高cs
	if(status==HAL_OK)
	{
		return 0;
	}
	else
	{
		return 1;
	}
			
}
uint8_t W25Q64_ReadStatus1(uint8_t *status_reg)
{
	uint8_t tx[2] = {0x05, 0x00};
    uint8_t rx[2];
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//拉低cs选中w25q64
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, 100);//发送同时接收
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//无论如何都拉高cs
	if(status==HAL_OK)
	{
		*status_reg=rx[1];
		return  0;
	}
	else{
		return 1;
	}	
}
uint8_t W25Q64_ReadJEDEC_ID(uint8_t *manufacturer,
                            uint8_t *memory_type,
                            uint8_t *capacity)
{   uint8_t tx[4] = {0x9F, 0x00, 0x00, 0x00};
    uint8_t rx[4];
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//拉低cs选中w25q64
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&hspi1, tx, rx, 4, 100);//发送同时接收
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//无论如何都拉高cs
	if(status==HAL_OK)
	{
		*manufacturer=rx[1];
		*memory_type=rx[2];
		*capacity=rx[3];
		return  0;
	}
	else{
		return 1;
	}	
	
}
uint8_t W25Q64_WaitBusy(void)
{   uint8_t status_reg;
	do{
		uint8_t status=W25Q64_ReadStatus1(&status_reg);//先执行一次判断读取是否成功
		if(status!=0)//读取失败
		{return 1;}
	}while(status_reg&0x01);//循坏时需要不断反馈而不是只读一次的结果
	
	 return 0;
	
}
uint8_t W25Q64_SectorErase(uint32_t address)
{uint8_t tx[4]={0x20,address>>16,address>>8,address};

  if(W25Q64_WaitBusy()==0)//查看是否忙
  {
	  if(W25Q64_WriteEnable()==0)//打开写使能用来修改flash内的内容
	  {
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//拉低cs选中w25q64开始擦除
		  HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, tx, 4, 100);//发送1位指令和3位地址
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//无论如何做完事先拉高cs
		  if(status==HAL_OK)//判断是否发送成功
		  {
			  if(W25Q64_WaitBusy()==0)//等待擦除完成  若不启动内部编程则不需要
			  {
				  return 0;
			  }
		  }
		  
	  }
  }
	return 1;
}
uint8_t W25Q64_ReadData(uint32_t address,uint8_t *data,uint16_t length)
{uint8_t cmd[4]={0x03,address>>16,address>>8,address};
  if(W25Q64_WaitBusy()==0)//查看是否忙
  {
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//拉低cs选中w25q64开始读取数据
		  HAL_StatusTypeDef status1 = HAL_SPI_Transmit(&hspi1, cmd, 4, 100);//发送1位指令和3位地址
	       if(status1==HAL_OK)//判断是否发送成功
	      {
		    HAL_StatusTypeDef status2 = HAL_SPI_Receive(&hspi1, data, length, 100);//在data地址中接收length长度的数据
			 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//无论如何做完事先拉高cs 
			  if(status2==HAL_OK)//判断是否接收成功
			  {
				   
				  return 0;
			  }
	       }
		  else
		  {
			   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//发送失败时主动拉高CS
			    return 1;
		  }
		  
  }
	return 1;
	
}
uint8_t W25Q64_PageProgram(uint32_t address,uint8_t *data,uint16_t length)
{uint8_t cmd[4] = {0x02,address >> 16,address >> 8,address};
if((length > 0) && (((address & 0xFF) + length) <= 256))//取地址第八位+长度不能大于256，跨页限制
	{
	if(W25Q64_WaitBusy()==0)//查看是否忙
  {
	  if(W25Q64_WriteEnable()==0)//打开写使能用来修改flash内的内容
	  {
		  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//拉低cs选中w25q64开始页编程
		  HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, cmd, 4, 100);//发送1位指令和3位地址
		  if(status==HAL_OK)//判断是否发送成功
		  {
			  HAL_StatusTypeDef status1 = HAL_SPI_Transmit(&hspi1, data, length, 100);//给出页编程需要的修改内容和长度
			  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//无论如何拉高cs
			  if(status1==HAL_OK)
			  {
				    if(W25Q64_WaitBusy()==0)//等待页编程完成  若不启动内部编程则不需要
			  {
				  return 0;
			  }
			  }
		  }
		  else{
			  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);//无论如何拉高cs
			  return 1;
		  }

		  }
	  }
  }
	return 1;
}
