/**
 * @file MB1_PWM.cpp
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 16-10-2014
 * @brief This is source file for PWM hardware for MBoard-1.
 */
#include "MB1_PWM.h"

using namespace pwm_ns;

TIM_TypeDef *timer_x[] = { TIM1, TIM8, TIM2, TIM3, TIM4, TIM5 };

const uint16_t sys_core_clock = 72; //MHz
/* TIM counter clock in PWM mode will run at 24MHz */
const uint16_t prescaler = (uint16_t) (sys_core_clock / 24) - 1;

pwm_hw::pwm_hw(void)
{
    this->timer = 0; //TIM1 as default.
    this->channel = 1;
    this->mode = TIM_OCMode_PWM1;
    this->period = 665;
    this->duty_cycle = 50;
}

bool pwm_hw::pwm_init(pwm_params_t *pwm_params)
{
    uint32_t timer_rcc;
    bool advanced_timer = false;

    /* save internal value */
    this->channel = pwm_params->pwm_channel;
    this->duty_cycle = pwm_params->duty_cycle;
    this->period = pwm_params->period;
    this->mode = pwm_params->pwm_mode;
    this->timer = (uint8_t) pwm_params->timer;

    switch (pwm_params->timer) {
    case adv_timer1:
        timer_rcc = RCC_APB2Periph_TIM1;
        advanced_timer = true;
        break;
    case adv_timer8:
        timer_rcc = RCC_APB2Periph_TIM8;
        advanced_timer = true;
        break;
    case gp_timer2:
        timer_rcc = RCC_APB1Periph_TIM2;
        break;
    case gp_timer3:
        timer_rcc = RCC_APB1Periph_TIM3;
        break;
    case gp_timer4:
        timer_rcc = RCC_APB1Periph_TIM4;
        break;
    case gp_timer5:
        timer_rcc = RCC_APB1Periph_TIM5;
        break;
    default:
        return false;
    }

    /* Timer RCC configuration */
    if (advanced_timer) {
        RCC_APB2PeriphClockCmd(timer_rcc, ENABLE);
    } else {
        RCC_APB1PeriphClockCmd(timer_rcc, ENABLE);
    }

    /* Timer base configuration */
    timer_base_init();

    /* PWMx mode configuration: Channel y */
    output_channel_init();

    TIM_ARRPreloadConfig(timer_x[timer], ENABLE);

    /* enable timer counter */
    TIM_Cmd(timer_x[timer], ENABLE);

    return true;
}

void pwm_hw::pwm_period_setup(uint16_t period)
{
    this->period = period; //update period.

    timer_base_init();

    pwm_duty_cycle_setup(this->duty_cycle);
}

void pwm_hw::pwm_duty_cycle_setup(uint8_t duty_cycle)
{
    this->duty_cycle = duty_cycle; //update duty cycle.

    output_channel_init();
}

void pwm_hw::timer_base_init(void)
{
    TIM_TimeBaseInitTypeDef time_base_init_struct;

    time_base_init_struct.TIM_Period = this->period;
    time_base_init_struct.TIM_Prescaler = prescaler;
    time_base_init_struct.TIM_ClockDivision = 0;
    time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(timer_x[timer], &time_base_init_struct);
}

void pwm_hw::output_channel_init(void)
{
    TIM_OCInitTypeDef oc_init_struct;

    oc_init_struct.TIM_OCMode = this->mode;
    oc_init_struct.TIM_Pulse = timer_pulse_convert(this->duty_cycle);
    oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
    oc_init_struct.TIM_OCPolarity = TIM_OCPolarity_High;
    switch (this->channel) {
    case 1:
        TIM_OC1Init(timer_x[timer], &oc_init_struct);
        TIM_OC1PreloadConfig(timer_x[timer], TIM_OCPreload_Enable);
        break;
    case 2:
        TIM_OC2Init(timer_x[timer], &oc_init_struct);
        TIM_OC2PreloadConfig(timer_x[timer], TIM_OCPreload_Enable);
        break;
    case 3:
        TIM_OC3Init(timer_x[timer], &oc_init_struct);
        TIM_OC3PreloadConfig(timer_x[timer], TIM_OCPreload_Enable);
        break;
    case 4:
        TIM_OC4Init(timer_x[timer], &oc_init_struct);
        TIM_OC4PreloadConfig(timer_x[timer], TIM_OCPreload_Enable);
        break;
    default:
        break;
    }
}

uint16_t pwm_hw::timer_pulse_convert(uint8_t duty_cycle)
{
    uint16_t ccr_value;
    uint8_t mod;

    ccr_value = (duty_cycle * (this->period)) / 100;
    mod = (duty_cycle * (this->period)) % 100;

    if (mod >= 50) {
        ccr_value++;
    }

    return ccr_value;
}
