/**
 * @file boardInteract.cc
 * @author Joe Rendleman (rendl008@umn.edu)
 * @brief Initializes GPIO pins and other necessary information based upon DIP
 * switch state.
 * Contains control functions for the Lights
 * @version 0.1
 * @date 2022-01-27
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "light.h"

namespace umnsvp {
namespace lights {

/**
 * @brief Construct a new Light:: Light object
 *
 * @param gpio_port GPIO PORT number
 * @param gpio_pin GPIO Pin number
 */

Light::Light(const uint16_t gpio_pin, GPIO_TypeDef* const gpio_port)
    : pin(gpio_pin), port(gpio_port) {
}

/**
 * @brief Initialize underlying GPIO configurations
 *
 * @param None
 */
void Light::init() {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    } else if (port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }

    // Pack the pin configuration into a struct.
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

    // Configure the GPIO port with the packed pin configuration.
    HAL_GPIO_Init(port, &GPIO_InitStruct);

    return;
}

/**
 * @brief Write ON state to light Pin
 *
 * @param None
 */
void Light::on() {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

/**
 * @brief Write OFF state to light Pin
 *
 * @param None
 */
void Light::off() {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

/**
 * @brief Toggle pin output
 *
 * @param None
 */
void Light::toggle() {
    HAL_GPIO_TogglePin(port, pin);
}

/**
 * @brief Read the current ouput condition of the light pin
 *
 * @return uint8_t true if ON
 */
bool Light::read() {
    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}

}  // namespace lights
}  // namespace umnsvp