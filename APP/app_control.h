#ifndef __APP_CONTROL_H
#define __APP_CONTROL_H
#include <stdint.h>

void App_ControlInit(void);
void App_ControlTask(void);
void App_ControlSetServoAngle(uint8_t angle);
uint8_t App_ControlGetServoAngle(void);
#endif
