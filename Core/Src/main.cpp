#include "clock_config.h"
#include "drv8825.hpp"
#include "gpio.h"
#include "gpio.hpp"
#include "pwm_device.hpp"
#include "tim.h"
#include "usart.h"
#include <utility>

int main()
{
    HAL_Init();
    SystemClock_Config();
    PeriphCommonClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_TIM3_Init();

    using namespace Utility;

    auto pwm_device = PWMDevice{&htim3, TIM_CHANNEL_4, 1U, 3.3F};

    auto drv8825 = DRV8825::DRV8825{std::move(pwm_device),
                                    GPIO::PA0,
                                    GPIO::PA1,
                                    GPIO::PA4,
                                    GPIO::PA5,
                                    GPIO::PA6,
                                    GPIO::PA7,
                                    GPIO::PA8,
                                    GPIO::PA9};

    drv8825.set_fast_decay();
    drv8825.set_half_microstep();
    drv8825.set_forward_direction();

    while (true) {
        drv8825.set_backward_direction();
        HAL_Delay(50UL);
        drv8825.set_forward_direction();
        HAL_Delay(50UL);
    }

    return 0;
}
