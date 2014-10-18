/**
 * @file MB1_GPIO.h
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 08-10-2014
 * @brief This is header file for interrupt handlers for MBoard-1.
 *
 * @How to use:
 *  _Declare an GPIO-class instance.
 *  _Assign values into a gpio_params_t struct.
 *  _Pass above struct into gpio_init() method (of the declared GPIO instance)
 *      to initialize GPIO.
 *  _If EXTI be used to, call exti_init() method to initialize EXTI.
 *  _If priority is important, lets call exti_priority_setup() method
 *      to set up priority.
 *  _Be able to enable/disable specified EXTI line and change trigger mode.
 */

#ifndef __MB1_GPIO_H_
#define __MB1_GPIO_H_

#include "MB1_Glb.h"

namespace gpio_ns {
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
    } exti_trigger_t;

    typedef enum {
        in_analog = 0x00,
        in_floating = 0x04,
        in_pull_down = 0x28,
        in_pull_up = 0x48,
        out_open_drain = 0x14,
        out_push_pull = 0x10,
        af_open_drain = 0x1C,
        af_push_pull = 0x18
    } gpio_mode_t;

    typedef struct {
        port_t port;
        uint8_t pin;
        gpio_mode_t mode;
    } gpio_params_t;
}

class gpio {
public:
    gpio(void);

    /**
     * @brief Initialize GPIO on the specified port/pin.
     *
     * @param[in] gpio_params The struct contains info about port/pin, mode of port/pin.
     *
     * @return true if success, false otherwise.
     */
    bool gpio_init(gpio_ns::gpio_params_t *gpio_params);

    /**
     * @frief Read level on the pin.
     *
     * @retrurn 0 if pin's in reset, #0 otherwise.
     */
    uint8_t gpio_read(void);

    /**
     * @brief Set pin. pin = 1;
     */
    void gpio_set(void);

    /**
     * @brief Clear pin. pin = 0;
     */
    void gpio_reset(void);

    /**
     * @brief Initialize EXTI on the specified port/pin.
     *
     * @param[in] trigger Trigger mode.
     *
     * @return true if success, false otherwise.
     */
    bool exti_init(gpio_ns::exti_trigger_t trigger);

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
    void exti_trigger_setup(gpio_ns::exti_trigger_t trigger);
private:
    gpio_ns::port_t exti_port_source;
    uint8_t exti_pin_source;
    uint16_t gpio_exti_pin;
    GPIO_TypeDef *gpio_x;
    bool in_mode;

    GPIO_InitTypeDef gpio_init_struct;
    EXTI_InitTypeDef exti_init_struct;
    NVIC_InitTypeDef nvic_init_struct;
};

#endif //__MB1_GPIO_H_
