/**
 * @file MB1_GPIO.cpp
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 08-10-2014
 * @brief This is source file for manage GPIO and its EXTI for MBoard-1.
 *
 */
#include "MB1_GPIO.h"

using namespace gpio_ns;

gpio::gpio(void)
{
    this->in_mode = true;
}

bool gpio::gpio_init(gpio_params_t *gpio_params)
{
    uint32_t periph_rcc;

    exti_port_source = gpio_params->port;
    exti_pin_source = gpio_params->pin;
    gpio_exti_pin = (uint16_t) 1 << exti_pin_source;

    switch (gpio_params->port) {
    case port_A:
        gpio_x = GPIOA;
        periph_rcc = RCC_APB2Periph_GPIOA;
        break;
    case port_B:
        gpio_x = GPIOB;
        periph_rcc = RCC_APB2Periph_GPIOB;
        break;
    case port_C:
        gpio_x = GPIOC;
        periph_rcc = RCC_APB2Periph_GPIOC;
        break;
    case port_D:
        gpio_x = GPIOD;
        periph_rcc = RCC_APB2Periph_GPIOD;
        break;
    case port_E:
        gpio_x = GPIOE;
        periph_rcc = RCC_APB2Periph_GPIOE;
        break;
    case port_F:
        gpio_x = GPIOF;
        periph_rcc = RCC_APB2Periph_GPIOF;
        break;
    case port_G:
        gpio_x = GPIOG;
        periph_rcc = RCC_APB2Periph_GPIOG;
        break;
    default:
        return false;
        break;
    }
    /* Enable peripheral clock */
    RCC_APB2PeriphClockCmd(periph_rcc, ENABLE);
    if (gpio_params->mode == af_open_drain
            || gpio_params->mode == af_push_pull) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    }

    if (gpio_params->mode == out_open_drain
            || gpio_params->mode == out_push_pull) {
        in_mode = false;
    }

    /* Initialize GPIO */
    gpio_init_struct.GPIO_Pin = gpio_exti_pin;
    gpio_init_struct.GPIO_Mode = (GPIOMode_TypeDef) gpio_params->mode;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio_x, &gpio_init_struct);

    return true;
}

uint8_t gpio::gpio_read(void)
{
    if (in_mode) {
        return GPIO_ReadInputDataBit(gpio_x, gpio_exti_pin);
    } else {
        return GPIO_ReadOutputDataBit(gpio_x, gpio_exti_pin);
    }
}

void gpio::gpio_set(void)
{
    GPIO_SetBits(gpio_x, gpio_exti_pin);
}

void gpio::gpio_reset(void)
{
    GPIO_ResetBits(gpio_x, gpio_exti_pin);
}

bool gpio::exti_init(exti_trigger_t trigger)
{
    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* Connect EXTI Line to "exti_gpio_pin" pin */
    GPIO_EXTILineConfig((uint8_t) exti_port_source, exti_pin_source);

    /* Initialize EXTI */
    exti_init_struct.EXTI_Line = gpio_exti_pin;
    exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init_struct.EXTI_Trigger = (EXTITrigger_TypeDef) trigger;
    exti_init_struct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_init_struct);

    switch (exti_pin_source) {
    case 0:
        nvic_init_struct.NVIC_IRQChannel = EXTI0_IRQn;
        break;
    case 1:
        nvic_init_struct.NVIC_IRQChannel = EXTI1_IRQn;
        break;
    case 2:
        nvic_init_struct.NVIC_IRQChannel = EXTI2_IRQn;
        break;
    case 3:
        nvic_init_struct.NVIC_IRQChannel = EXTI3_IRQn;
        break;
    case 4:
        nvic_init_struct.NVIC_IRQChannel = EXTI4_IRQn;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        nvic_init_struct.NVIC_IRQChannel = EXTI9_5_IRQn;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        nvic_init_struct.NVIC_IRQChannel = EXTI15_10_IRQn;
        break;
    default:
        return false;
    }
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic_init_struct.NVIC_IRQChannelSubPriority = 0x0F;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);

    return true;
}

void gpio::exti_line_enable(void)
{
    exti_init_struct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_init_struct);
}

void gpio::exti_line_disable(void)
{
    exti_init_struct.EXTI_LineCmd = DISABLE;
    EXTI_Init(&exti_init_struct);
}

void gpio::exti_trigger_setup(exti_trigger_t trigger)
{
    exti_init_struct.EXTI_Trigger = (EXTITrigger_TypeDef) trigger;
    EXTI_Init(&exti_init_struct);
}

void gpio::exti_priority_setup(uint8_t preemption, uint8_t sub)
{
    nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemption;
    nvic_init_struct.NVIC_IRQChannelSubPriority = sub;
    nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_struct);
}
