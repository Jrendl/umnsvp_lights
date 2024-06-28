/**
 * @file main.cc
 * @author Joe Rendleman (rendl008@umn.edu)
 * @brief Lights main task.
 * @version 0.1
 * @date 2022-01-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "main.h"

#include "app.h"
umnsvp::lights::Application app;
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void) {
    /*
     * Create and run the main application.
     */
    app.main();
}

void timer_handler_callback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM16) {
        app.send_status();
        app.send_ID();
    }
    if (htim->Instance == TIM7) {
        app.blinky_handler();
    }
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */

    /* Infinite loop */
    while (1) {
    }
}

#endif

extern "C" void CAN1_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(app.get_handle());
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    app.can_rx_callback();
}

extern "C" void CAN1_TX_IRQHandler(void) {
    app.can_tx_callback();
}

extern TIM_HandleTypeDef htim16;
extern "C" void TIM1_UP_TIM16_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim16);
}

extern TIM_HandleTypeDef htim7;
extern "C" void TIM7_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim7);
}
