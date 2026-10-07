/* app_rtos.h */

#ifndef APP_RTOS_H
#define APP_RTOS_H

#include "cmsis_os2.h"
#define CMD_RX_FLAG    (1U << 0)
extern volatile uint32_t control_queue_drop_count;//这个变量确实存在，但不是我这里创建的，它在别的 .c 里面，你链接的时候去找
extern osThreadId_t CommandTaskHandle;
extern osMutexId_t uartMutexHandle;
extern osMessageQueueId_t controlQueueHandle;
extern osMutexId_t i2cMutexHandle;
typedef enum
{
    CTRL_SERVO_SET = 1,
    CTRL_SAVE,
    CTRL_LOAD,
	CTRL_ENCODER_STEP
} ControlCmd_t;  //枚举1，2，3

typedef struct
{
    ControlCmd_t cmd;
    int32_t value;
} ControlMsg_t;
#endif      //命令，值
