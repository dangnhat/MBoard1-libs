/**
 * @file MB1_EXTI.h
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 08-10-2014
 * @brief This is header file for interrupt handlers for MBoard-1.
 *
 * @How to use:
 *  _Declare an EXTI-line instance.
 *  _Assign values into a exti_params_t struct.
 *  _Pass above struct into exti_init() method (of the declared EXTI instance)
 *      to initialize EXTI.
 *  _If priority is important, lets call exti_priority_setup() method
 *      to set up priority.
 *  _Be able to enable/disable specified EXTI line and change trigger mode.
 */

#ifndef __MB1_EXTI_H_
#define __MB1_EXTI_H_

#include "MB1_Glb.h"

namespace exti_ns {
    typedef enum {
        port_A = 0,
        port_B = 1,
        port_C = 2,
        port_D = 3,
        port_E = 4,
        port_F = 5,
        port_G = 6
    } port_t;

    typedef enum {
        rising_edge = 0x08,
        falling_edge = 0x0C,
        both_edge = 0x10
    } trigger_t;

    typedef enum {
        floating = 0x04,
        pull_down = 0x28,
        pull_up = 0x48
    } gpio_mode_t;

    typedef struct {
        port_t port;
        gpio_mode_t mode;
        trigger_t trigger;
    } exti_params_t;
}

class exti {
public:
    /**
     * @brief Get EXTI line and GPIO pin source from "exti_line".
     *
     * @param[in] exti_line 0->15.
     */
    exti(uint8_t exti_line);

    /**
     * @brief Initialize EXTI on the specified port/pin.
     *
     * @param[in] params_struct Contains port, mode of the port and trigger mode.
     *
     * @return true if success, false otherwise.
     */
    bool exti_init(exti_ns::exti_params_t *params_struct);

    /**
     * @brief Enable external interrupt line.
     */
    void exti_line_enable(void);

    /**
     * @brief Disable external interrupt line.
     */
    void exti_line_disable(void);

    /**
     * @brief Set up preemption and sub priority. (This method'll enable EXTI irq).
     *
     * @param[in] preemption    Preemption priority.
     * @param[in] sub           Sub priority.
     */
    void exti_priority_setup(uint8_t preemption, uint8_t sub);

    /**
     * @brief Set up the trigger mode (falling, rising or both).
     *
     * @param[in] trigger Trigger mode.
     */
    void exti_trigger_setup(exti_ns::trigger_t trigger);
private:
    uint8_t exti_pin_source;
    uint16_t exti_gpio_pin;

    GPIO_InitTypeDef gpio_init_struct;
    EXTI_InitTypeDef exti_init_struct;
    NVIC_InitTypeDef nvic_init_struct;
};

#endif //__MB1_EXTI_H_
