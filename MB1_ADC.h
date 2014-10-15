/**
 * @file MB1_ADC.h
 * @author  Nguyen Van Hien <nvhien1992@gmail.com>, HLib MBoard team.
 * @version 1.0
 * @date 14-10-2014
 * @brief This is header file for ADC conversion for MBoard-1.
 *
 * @How to use:
 */
#ifndef __MB1_ADC_H_
#define __MB1_ADC_H_

#include "MB1_Glb.h"

namespace adc_ns {
typedef enum {
    adc1,
    adc2,
    adc3
} adc_t;

typedef enum {
    independent,
    dual_adc
} adc_mode_t;

typedef enum {
    single_mode,
    continuous_mode,
    discontinuous_mode,
    scan_mode
} adc_conv_mode_t;

typedef enum {
    ext_trigger,
    analog_watchdog,
    no_option
} adc_option_t;

typedef enum {
    dma_request,
    irq,
    poll
} adc_access_t;

typedef enum {
    regular_channel,
    injected_channel
} adc_chn_t;

typedef struct {
    adc_mode_t adc_mode;
    adc_t adc;
    adc_conv_mode_t conv_mode;
    adc_chn_t channel_type;
    adc_option_t option;
    uint8_t adc_sample_time;
    adc_access_t data_access;
} adc_params_t;
}

class adc {
public:
    adc(uint8_t channel);

    void adc_init(adc_ns::adc_params_t *adc_params);
    void adc_start(void);
    void adc_stop(void);
    uint16_t adc_convert(void);
private:
    uint8_t channel;
    adc_ns::adc_t adc_num;
    uint32_t adc1_dr_addr = 0x4001244C;
    uint32_t adc3_dr_addr = 0x40013C4C;
    uint16_t adc_converted_value;
    ADC_InitTypeDef adc_init_struct;

    void dma_config(DMA_Channel_TypeDef *dma_channel_x, uint32_t adc_dr_addr);
};

#endif //__MB1_ADC_H_
