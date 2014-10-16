/**
 * @file MB1_PWM.h
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 16-10-2014
 * @brief This is header file for PWM hardware for MBoard-1.
 * @How to use:
 *  -Declare an adc-class instance with a channel ADC.
 *  -Initialize GPIO as analog input for ADC channel.
 *  -Enter value to an "adc_params_t"
 *  -Pass above struct into adc_init method to initialize ADC.
 *  -Call adc_convert method to get converted data.
 *  Note: DMA can only serve ADC1 and ADC3. ADCCLK: 0.6->14MHz.
 */
#ifndef __MB1_PWM_H_
#define __MB1_PWM_H_

#include "MB1_Glb.h"

namespace pwn_ns {

}

class pwm_hw {
public:
private:
};

#endif //__MB1_PWM_H_
