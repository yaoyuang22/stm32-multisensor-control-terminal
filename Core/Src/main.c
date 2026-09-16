/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uart.h"
#include <stdio.h>
#include "app_command.h"
#include "app_monitor.h"
#include "app_control.h"
#include "app_config.h"
#include "bsp_led.h"
#include "bsp_oled.h"
#include "bsp_sht30.h"
#include "bsp_servo.h"
#include "bsp_w25q64.h"
#include "bsp_encoder.h"
#include "bsp_key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define DMA_RX_SIZE 64
#define ADC_BUF_SIZE 16
static uint16_t adc_buffer[ADC_BUF_SIZE];//ADC是0~4095 也就是2的12次方 uint8_t装不下 16的话就0~65535 对应HALF WORD 半字=2byte=16bit
static volatile uint8_t timer_flag = 0;
static uint8_t dma_rx_buf[DMA_RX_SIZE];
volatile uint8_t key_flag = 0;
volatile uint8_t key_locked = 0;
volatile uint32_t ic_value = 0;
volatile uint8_t ic_flag = 0;
volatile uint32_t ic_last = 0;
volatile uint32_t ic_period = 0;
volatile uint8_t ic_first = 1;
volatile uint8_t ic_period_flag = 0;
volatile uint32_t ic_rise = 0;
volatile uint32_t ic_last_rise = 0;
volatile uint32_t ic_high_width = 0;
volatile uint8_t ic_have_last_rise = 0;
volatile uint8_t ic_wait_falling = 0;
volatile uint8_t ic_result_flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
OLED_Init();
HAL_TIM_Base_Start_IT(&htim3);//“启动 TIM3 基本定时器，并开启中断
HAL_UARTEx_ReceiveToIdle_DMA(&huart1,dma_rx_buf,sizeof(dma_rx_buf));//启动 DMA 接收，直到串口空闲时或者接收数组满时停止并进入返回函数HAL_UARTEx_RxEventCallback
__HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);//阻止HAL_UARTEx_ReceiveToIdle_DMA在存到一半时进入HAL_UARTEx_RxEventCallback返回函数
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);//开启TIM2和TIM2通道2的PWM
HAL_ADCEx_Calibration_Start(&hadc1);//开启ADC转换之前先进行ADC校准
HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_buffer,ADC_BUF_SIZE);//启动 ADC1，并使用它所关联的 DMA，把 ADC 转换结果连续存入 adc_buffer，一轮长度为 ADC_BUF_SIZE。DMA和usart1 rx的channel不一样
App_ControlInit();
App_ConfigLoad();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 
  {  
/* ================= UART TASK ================= */
Command_Task();
/* ================= ENCODER TASK ================= */  
App_ControlTask();

/* ================= KEY TASK ================= */
Key_Task();//负责“检测松开稳定50ms、解除锁定”
if(Key_GetPressEvent())     //一次有效按键事件
{
    App_ConfigSave();//按下按键后将数据存入w25q64中
}
/* ================= 1s TASK ================= */ 	
App_MonitorTask(adc_buffer, ADC_BUF_SIZE);  
	 	
}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //外部中断时触发的返回函数
{  
	/* 原来的PB1按键 */
    if(GPIO_Pin == GPIO_PIN_1)
    {
          Key_EXTI_Handler();
    }
	
	/* 编码器A相 PB10 */
    else if(GPIO_Pin == GPIO_PIN_10)
{
    Encoder_EXTI_Handler();
}

}	
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器达到时间时触发的返回函数
{
    if(htim->Instance==TIM3)
	{
		App_MonitorTick();
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,    //UART返回函数
                                uint16_t Size)
{   uint16_t i;
    if(huart->Instance == USART1)
	{  
		for(i = 0; i < Size; i++)
    {
         RingBuffer_Write(dma_rx_buf[i]);
    }
		HAL_UARTEx_ReceiveToIdle_DMA(huart,dma_rx_buf,sizeof(dma_rx_buf));
	  __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
    }
	
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
