/**
 * @file MB1_ADC.cpp
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 14-10-2014
 * @brief This is source file for ADC conversion for MBoard-1.
 *
 */
#include "MB1_ADC.h"

using namespace adc_ns;

uint16_t adc_converted_value;

adc::adc(void)
{
    this->poll_data = false;
    /* ADC1 as default */
    this->adc_x = ADC1;
}

void adc::adc_init(adc_params_t *adc_params)
{
    switch (adc_params->adc) {
    case adc1:
        adc_x = ADC1;
        adc_rcc = RCC_APB2Periph_ADC1;
        break;
    case adc2:
        adc_x = ADC2;
        adc_rcc = RCC_APB2Periph_ADC2;
        break;
    case adc3:
        adc_x = ADC3;
        adc_rcc = RCC_APB2Periph_ADC3;
        break;
    default:
        return;
    }

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_APB2PeriphClockCmd(adc_rcc, ENABLE);

    switch (adc_params->adc_mode) {
    case independent:
        adc_init_struct.ADC_Mode = ADC_Mode_Independent;
        break;
    case dual_adc:
        //TODO
        break;
    default:
        return;
    }

    switch (adc_params->conv_mode) {
    case single_mode:
        adc_init_struct.ADC_ContinuousConvMode = DISABLE;
        adc_init_struct.ADC_ScanConvMode = DISABLE;
        adc_init_struct.ADC_NbrOfChannel = 1;
        break;
    case continuous_mode:
        adc_init_struct.ADC_ContinuousConvMode = ENABLE;
        adc_init_struct.ADC_ScanConvMode = DISABLE;
        adc_init_struct.ADC_NbrOfChannel = 1;
        break;
    case discontinuous_mode:
        //TODO
        break;
    case scan_mode:
        //TODO
        break;
    default:
        return;
    }

    switch (adc_params->option) {
    case ext_trigger:
        //TODO
        break;
    case analog_watchdog:
        //TODO
        break;
    case no_option:
        adc_init_struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
        break;
    default:
        return;
    }

    switch (adc_params->data_access) {
    case dma_request:
        break;
    case adc_irq:
        //TODO
        break;
    case poll:
        poll_data = true;
        break;
    default:
        return;
    }

    adc_init_struct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init(adc_x, &adc_init_struct);

    switch (adc_params->channel_type) {
    case regular_channel:
        ADC_RegularChannelConfig(adc_x, adc_params->adc_channel, 1,
                adc_params->adc_sample_time);
        break;
    case injected_channel:
        //TODO
        break;
    default:
        return;
    }
}

void adc::adc_start(void)
{
    adc_en_dis(ENABLE);
}

void adc::adc_stop(void)
{
    adc_en_dis(DISABLE);
}

uint16_t adc::adc_convert(void)
{
    /* Start conversation */
    ADC_SoftwareStartConvCmd(adc_x, ENABLE);
    if (poll_data) {
        while (!ADC_GetFlagStatus(adc_x, ADC_FLAG_EOC)) {
            ;
        }
        adc_converted_value = ADC_GetConversionValue(adc_x);
        ADC_ClearFlag(adc_x, ADC_FLAG_EOC);
    }
    return adc_converted_value;
}

void adc::adc_en_dis(FunctionalState new_state)
{
    RCC_APB2PeriphClockCmd(adc_rcc, new_state);
    ADC_Cmd(adc_x, new_state);

    if (new_state == ENABLE) {
        adc_calibration();
    }
}

void adc::adc_calibration(void)
{
    ADC_ResetCalibration(adc_x);
    while (ADC_GetResetCalibrationStatus(adc_x)) {
        ;
    }

    ADC_StartCalibration(adc_x);
    while (ADC_GetCalibrationStatus(adc_x)) {
        ;
    }
}
