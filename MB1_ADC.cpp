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

adc::adc(uint8_t channel) {
    this->channel = channel;
    this->adc_converted_value = 0;
    /* ADC1 as default */
    this->adc_num = adc1;
}

void adc::adc_init(adc_params_t *adc_params) {
    uint32_t adc_dr_address;
    ADC_TypeDef *adc_x;
    DMA_Channel_TypeDef *dma_channel_x;
    this->adc_num = adc_params->adc;

    switch (adc_params->adc) {
    case adc1:
        adc_x = ADC1;
        adc_dr_address = adc1_dr_addr;
        dma_channel_x = DMA1_Channel1;
        break;
    case adc2:
        adc_x = ADC2;
        break;
    case adc3:
        adc_x = ADC3;
        adc_dr_address = adc3_dr_addr;
        dma_channel_x = DMA2_Channel1;
        break;
    default:
        break;
    }

    switch (adc_params->adc_mode) {
    case independent:
        adc_init_struct.ADC_Mode = ADC_Mode_Independent;
        break;
    case dual_adc:
        //TODO
        break;
    default:
        break;
    }

    switch(adc_params->conv_mode) {
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
        break;
    }

    switch(adc_params->option) {
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
        break;
    }

    switch(adc_params->data_access) {
    case dma_request:
        dma_config(dma_channel_x, adc_dr_address);
        ADC_DMACmd(adc_x, ENABLE);
        break;
    case irq:
        //TODO
        break;
    case poll:
        break;
    }

    adc_init_struct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init(adc_x, &adc_init_struct);

    switch(adc_params->channel_type) {
    case regular_channel:
        ADC_RegularChannelConfig(adc_x, channel, 1, adc_params->adc_sample_time);
        break;
    case injected_channel:
        //TODO
        break;
    default:
        break;
    }

}

void adc::adc_start(void) {
    ADC_TypeDef *adc_x;

    switch (adc_num) {
    case adc1:
        adc_x = ADC1;
        break;
    case adc2:
        adc_x = ADC2;
        break;
    case adc3:
        adc_x = ADC3;
        break;
    default:
        break;
    }

    ADC_Cmd(adc_x, ENABLE);
}

void adc::adc_stop(void) {
    ADC_TypeDef *adc_x;

    switch (adc_num) {
    case adc1:
        adc_x = ADC1;
        break;
    case adc2:
        adc_x = ADC2;
        break;
    case adc3:
        adc_x = ADC3;
        break;
    default:
        break;
    }

    ADC_Cmd(adc_x, DISABLE);
}

uint16_t adc::adc_convert(void) {
    return adc_converted_value;
}

void adc::dma_config(DMA_Channel_TypeDef *dma_channel_x, uint32_t adc_dr_addr) {
    DMA_InitTypeDef dma_init_struct;

    DMA_DeInit(dma_channel_x);
    dma_init_struct.DMA_PeripheralBaseAddr = adc_dr_addr;
    dma_init_struct.DMA_MemoryBaseAddr = (uint32_t)&adc_converted_value;
    dma_init_struct.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma_init_struct.DMA_BufferSize = 1;
    dma_init_struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma_init_struct.DMA_MemoryInc = DMA_MemoryInc_Disable;
    dma_init_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma_init_struct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma_init_struct.DMA_Mode = DMA_Mode_Circular;
    dma_init_struct.DMA_Priority = DMA_Priority_High;
    dma_init_struct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(dma_channel_x, &dma_init_struct);
    DMA_Cmd(dma_channel_x, ENABLE);
}
