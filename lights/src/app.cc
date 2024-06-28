#include "app.h"

#include "main.h"
#include "timing.h"
namespace umnsvp {
namespace lights {
Application::Application()
    : head_light(light_0),
      high_beam(light_1),
      front_left(light_2),
      front_right(light_3),
      back_left(light_1),
      back_right(light_2),
      blink_light(light_3),
      brake(light_0),
      camera(light_5),
      skylab(CAN1) {
    front = false;
    bms_fault = false;
}

/**
 * @brief Main method of Lights application
 */
void Application::main() {
    init();
    // Check front or Back
    if (front) {
        // front: Headlights:0, High Beams:1, Left Turn:2, Right Turn:3
        // Camera: 5

        // forever loop
        while (1) {
            // check headlights
            if (skylab.vision_headlights_command_buffer.pop()) {
                skylab2::can_packet_vision_headlights_command cmd =
                    skylab.vision_headlights_command_buffer.output();
                if (cmd.lights.high_beams) {
                    high_beam.on();
                } else {
                    high_beam.off();
                }

                if (cmd.lights.headlights) {
                    head_light.on();
                } else {
                    head_light.off();
                }
            }

            // check turn signals
            if (skylab.vision_turn_signals_command_buffer.pop()) {
                skylab2::can_packet_vision_turn_signals_command cmd =
                    skylab.vision_turn_signals_command_buffer.output();
                if (cmd.lights.left_turn_signal) {
                    front_left.on();
                } else {
                    front_left.off();
                }
                if (cmd.lights.right_turn_signal) {
                    front_right.on();
                } else {
                    front_right.off();
                }
            }
        }
    } else {
        // Back: Brake Lights:0, Left Turn:1 Right Turn:2,
        // Blinky Light:3, Camera:5

        // init can bus

        // forever loop
        while (1) {
            if (skylab.vision_brake_lights_command_buffer.pop()) {
                skylab2::can_packet_vision_brake_lights_command cmd =
                    skylab.vision_brake_lights_command_buffer.output();
                if (cmd.lights.brake_lights) {
                    brake.on();
                } else {
                    brake.off();
                }
            }

            // check turn signals
            if (skylab.vision_turn_signals_command_buffer.pop()) {
                skylab2::can_packet_vision_turn_signals_command cmd =
                    skylab.vision_turn_signals_command_buffer.output();
                if (cmd.lights.left_turn_signal) {
                    back_left.on();
                } else {
                    back_left.off();
                }
                if (cmd.lights.right_turn_signal) {
                    back_right.on();
                } else {
                    back_right.off();
                }
            }

            // battery fault indicator
            if (skylab.bms_kill_reason_buffer.pop()) {
                bms_fault = true;
            }
        }
    }
}

/**
 * @brief Application initialization for Hardware related interfaces
 *
 */
void Application::init() {
    /* STM32L4xx HAL library initialization:
     * - Configure the Flash prefetch
     * - Systick timer is configured by default as source of time
     * base, but user can eventually implement his proper time base
     * source (a general purpose timer for example or other time
     * source), keeping in mind that Time base duration should be
     * kept 1ms since PPP_TIMEOUT_VALUEs are defined and handled in
     * milliseconds basis.
     * - Set NVIC Group Priority to 4
     * - Low Level Initialization
     */

    HAL_Init();

    /* Configure the system clock to 80 MHz */
    System_Clock_Config();

    skylab.init();

    // initialize telemetry manager

    // initialize hardware interface
    start_CAN_timer(&timer_handler_callback);
    start_blinky_timer(&timer_handler_callback);
    // read DIP switch and set FRONT
    dip.init();

    front = dip.get_state();
    // initialize GPIO ports

    if (front) {
        // front: Headlights:0, High Beams:1, Left Turn:2, Right Turn:3,
        // Camera:
        // 5
        head_light.init();
        high_beam.init();
        front_left.init();
        front_right.init();

    } else {
        // Back: Brake Lights:0, Left Turn:1, Right Turn:2,
        // Blinky Light:3, Camera:5
        brake.init();
        back_left.init();
        back_right.init();
        blink_light.init();
    }
    // write on state to cameras

    camera.init();

    camera.on();

    // 12V en

    enable.init();
    enable.on();

    return;
}

void Application::blinky_handler(void) {
    if (bms_fault) {
        blink_light.toggle();
    }
}

/**
 * @brief rx callback function for skylab2
 *
 */
void Application::can_rx_callback(void) {
    skylab.rx_handler();
}

void Application::can_tx_callback(void) {
    skylab.tx_handler();
}

CAN_HandleTypeDef* Application::get_handle() {
    return skylab.get_handle();
}

void Application::send_status() {
    if (front) {
        skylab2::can_packet_vision_status_front status_msg =
            skylab2::can_packet_vision_status_front();
        status_msg.lights.headlights = head_light.read();
        status_msg.lights.high_beams = high_beam.read();
        status_msg.lights.left_turn_signal = front_left.read();
        status_msg.lights.right_turn_signal = front_right.read();

        skylab.send_vision_status_front(status_msg);
    } else {
        skylab2::can_packet_vision_status_rear status_msg =
            skylab2::can_packet_vision_status_rear();
        status_msg.lights.brake_lights = brake.read();
        status_msg.lights.left_turn_signal = back_left.read();
        status_msg.lights.right_turn_signal = back_right.read();

        skylab.send_vision_status_rear(status_msg);
    }
}

void Application::send_ID() {
    static skylab2::can_packet_lights_front_id front_id =
        skylab2::can_packet_lights_front_id();
    static skylab2::can_packet_lights_back_id back_id =
        skylab2::can_packet_lights_back_id();

    if (front) {
        skylab.send_lights_front_id(front_id);
    } else {
        skylab.send_lights_back_id(back_id);
    }
}

}  // namespace lights
}  // namespace umnsvp