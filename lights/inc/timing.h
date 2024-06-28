#pragma once
#include "stm32l4xx_hal.h"

void start_CAN_timer(pTIM_CallbackTypeDef USER_TIM_PeriodElapsedCallback);
void start_blinky_timer(pTIM_CallbackTypeDef USER_TIM_PeriodElapsedCallback);