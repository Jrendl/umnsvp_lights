#pragma once

#include <stm32l4xx_hal.h>

#include "application_base.h"
#include "dip_switch.h"
#include "light.h"
#include "skylab2.h"

namespace umnsvp {
namespace lights {

class Application : public ApplicationBase {
   public:
    Application();
    void main(void);
    void send_status(void);
    void send_ID(void);
    void blinky_handler(void);

    Light &head_light;
    Light &high_beam;
    Light &front_left;
    Light &front_right;
    Light &back_left;
    Light &back_right;
    Light &blink_light;
    Light &brake;
    Light &camera;

    void can_rx_callback(void);
    void can_tx_callback(void);

    CAN_HandleTypeDef *get_handle();

   private:
    void init();
    void system_clock_config(void);
    bool front;
    bool bms_fault;

    Light light_0 = Light(PIN_LIGHT_0, PORT_LIGHT_0);
    Light light_1 = Light(PIN_LIGHT_1, PORT_LIGHT_1);
    Light light_2 = Light(PIN_LIGHT_2, PORT_LIGHT_2);
    Light light_3 = Light(PIN_LIGHT_3, PORT_LIGHT_3);
    Light light_4 = Light(PIN_LIGHT_4, PORT_LIGHT_4);
    Light light_5 = Light(PIN_LIGHT_5, PORT_LIGHT_5);

    Light enable = Light(PIN_12V_EN, PORT_12V_EN);

    umnsvp::skylab2::Skylab2 skylab;
    umnsvp::dip_switch dip = dip_switch(PORT_DIP, PIN_DIP);
};

}  // namespace lights
}  // namespace umnsvp