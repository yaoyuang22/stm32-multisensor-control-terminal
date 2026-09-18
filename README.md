# STM32 Multi-Sensor Control Terminal

基于 STM32F103C8T6 开发的多传感器监测与执行控制终端。

## 项目简介

本项目基于 STM32F103C8T6，使用 STM32 HAL 库开发，实现温湿度采集、ADC 模拟量采集、OLED 显示、串口命令交互、编码器控制舵机以及 W25Q64 参数掉电保存等功能。

软件采用 BSP / APP / Main 分层结构，并在裸机版本中使用非阻塞 Super Loop 进行任务调度。

目前正在进一步迁移至 FreeRTOS 多任务架构。

## 硬件平台

- STM32F103C8T6
- SHT30 温湿度传感器
- SSD1306 OLED
- W25Q64 SPI Flash
- EC11 旋转编码器
- SG90 舵机
- USB-TTL / CH340
- 逻辑分析仪

## 主要功能

- UART DMA + IDLE 不定长数据接收
- RingBuffer 串口数据缓存
- CR/LF 命令组帧与解析
- SHT30 温湿度采集及 CRC8 校验
- ADC + DMA 循环采样及平均滤波
- OLED 状态显示
- EC11 编码器控制舵机角度
- TIM PWM 驱动 SG90
- W25Q64 参数保存与掉电恢复
- 按键 EXTI 与软件消抖
- BSP / APP 模块化设计

## 软件架构

### 裸机版本

```text
              Main Super Loop
                       |
        --------------------------------
        |              |               |
   app_command     app_control     app_monitor
        |              |               |
        --------------- APP -------------
                       |
                      BSP
                       |
    -------------------------------------------
    |        |        |        |             |
   ADC     Servo     SHT30    W25Q64     Encoder/Key
    |        |        |        |             |
    ------------------ HAL --------------------
                       |
                 STM32 Hardware

```
## 关键技术

### UART DMA + IDLE + RingBuffer

USART1 使用 DMA 接收不定长数据，通过 IDLE 事件获取本次接收长度，
并将数据写入 RingBuffer，由应用层完成 CR/LF 命令组帧和解析。

### ADC + DMA

ADC1_IN0 使用 DMA Circular 模式连续采样，
通过多点平均获得模拟量数据，避免主循环阻塞等待 ADC 转换。

### W25Q64 参数持久化

通过 SPI 驱动 W25Q64，实现舵机角度配置的保存和恢复。
配置数据包含 Magic、Version 和 ServoAngle，用于判断 Flash 数据是否合法。

### 软件分层

项目按照 APP / BSP / HAL 分层：
- APP：业务逻辑和系统状态
- BSP：具体硬件操作接口
- HAL：STM32 外设驱动

避免应用层直接操作底层硬件状态。
## FreeRTOS 迁移进度

- [√] Task / Scheduler / Tick
- [√] MonitorTask
- [ ] CommandTask
- [ ] ControlTask
- [ ] Queue
- [ ] Mutex
