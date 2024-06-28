#include "timing.h"

TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim7;

void start_CAN_timer(pTIM_CallbackTypeDef USER_TIM_PeriodElapsedCallback) {
    // Timer 16 uses APB1 clock source for internal clock
    // Set priorities to 4 for higher priority compared to the other interupts
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 8, 8);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    __HAL_RCC_TIM16_CLK_ENABLE();

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim16.Instance = TIM16;
    // This clock setup is for an interupt every 500ms with an APB clock of 80
    // mHZ
    htim16.Init.Prescaler = 40000;
    htim16.Init.Period = 10000;
    htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    // Init timer
    if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
        while (1)
            ;
    }
    // Register the timer callback function
    if (HAL_TIM_RegisterCallback(&htim16, HAL_TIM_PERIOD_ELAPSED_CB_ID,
                                 USER_TIM_PeriodElapsedCallback) != HAL_OK) {
        while (1)
            ;
    }
    // Clear the IT flag befroe starting the timer interupt
    __HAL_TIM_CLEAR_IT(&htim16, TIM_IT_UPDATE);
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim16, &sMasterConfig) !=
        HAL_OK) {
        while (1)
            ;
    }
    // Start the timer interupt
    HAL_TIM_Base_Start_IT(&htim16);

    return;
}

void start_blinky_timer(pTIM_CallbackTypeDef USER_TIM_PeriodElapsedCallback) {
    // Timer 7 uses APB1 clock source for internal clock
    // Set priorities to 4 for higher priority compared to the interupts
    HAL_NVIC_SetPriority(TIM7_IRQn, 4, 4);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    __HAL_RCC_TIM7_CLK_ENABLE();

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim7.Instance = TIM7;
    // This clock setup is for an interupt every 1s with an APB clock of 80
    // mHZ
    htim7.Init.Prescaler = 40000;
    htim7.Init.Period = 1000;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    // Init timer
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
        while (1)
            ;
    }
    // Register the timer callback function
    if (HAL_TIM_RegisterCallback(&htim7, HAL_TIM_PERIOD_ELAPSED_CB_ID,
                                 USER_TIM_PeriodElapsedCallback) != HAL_OK) {
        while (1)
            ;
    }
    // Clear the IT flag befroe starting the timer interupt
    __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) !=
        HAL_OK) {
        while (1)
            ;
    }
    // Start the timer interupt
    HAL_TIM_Base_Start_IT(&htim7);

    return;
}