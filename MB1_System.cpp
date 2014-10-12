/**
 * @file MB1_System.cpp
 * @author  Pham Huu Dang Nhat  <phamhuudangnhat@gmail.com>, HLib MBoard team.
 * @version 1.2
 * @date 9-4-2014
 * @brief This file implement MB1_system_init function.
 * MB1_System :
 *
 * (MB1_SPI1)       (MB1_SPI2)
 *
 * (MB1_Led_green)  (MB1_Led_red)
 *
 * (MB1_UsrBtn0)    (MB1_UsrBtn1)
 * btn_activeStates = 0,
 * btn_samplingTimeCycle = 5 * miscTIM_period
 * btn_longPressedTime = 200 * btn_samplingTimeCycle;
 *
 * (MB1_USART1)
 * baud_rate (9600), retarget (config_interface)
 *
 * (MB1_USART2)
 * baud_rate (9600), retarget (config_interface)
 *
 * (Misc functions)
 * ledBeat_period, miscTIM_period (TIM6, 1msec).
 *
 * (MB1_ISRs)
 * TIM6_ISRs                    other ISR
 * | LedBeat_ISR    |           | subISR_ptr    |
 * | delay_ms_ISR   |           | subISR_ptr    |
 * | btn_ISR        |           | subISR_ptr    |
 * | ............   |           | ............  |
 * g_numOfSubISR_max (default = 4)
 *
 * (NVIC)
 * 2 bit for preemption priority
 * 2 bit for sub priority
 *
 * Other notes :
 * - bugs_fix.
 * - no NJTRST.
 */

 #include "MB1_System.h"

/**<-------------- Global vars and objects in the system of MB1 ------------*/

/** EXTIs */
exti MB1_EXTI0(0);
exti MB1_EXTI1(1);
exti MB1_EXTI2(2);
exti MB1_EXTI3(3);
exti MB1_EXTI4(4);
exti MB1_EXTI5(5);
exti MB1_EXTI6(6);
exti MB1_EXTI7(7);
exti MB1_EXTI8(8);
exti MB1_EXTI9(9);
exti MB1_EXTI10(10);
exti MB1_EXTI11(11);
exti MB1_EXTI12(12);
exti MB1_EXTI13(13);
exti MB1_EXTI14(14);
exti MB1_EXTI15(15);

/**< SPIs */
SPI MB1_SPI1 (1);
SPI MB1_SPI2 (2);

/**< LEDs */
Led MB1_Led_green (Led_ns::green);
Led MB1_Led_red (Led_ns::red);

/**< Buttons */
Button MB1_usrBtn0 (Btn_ns::usrBtn_0);
Button MB1_usrBtn1 (Btn_ns::usrBtn_1);

/**< USARTs */
/* serial_t MB1_USART1 (1); */
serial_t MB1_USART2 (2);

/**< CRC */
crc_hw MB1_crc;

/**< Misc, global functions ! */

/**< ISRs */
ISRMgr MB1_ISRs;

/**< NVIC */
const uint32_t MB1_NVIC_PriorityGroup = NVIC_PriorityGroup_2;

/**< Vector table relocation */
const uint32_t MB1_VectorTableRelocationOffset = 0x3000;
const bool MB1_VectorTableRelocation_isUsed = false;


/**<-------------- Global vars and objects in the system of MB1 ------------*/

/**< conf interface (compile-time) */

/**< for SysTick, miscTIM and led_beat */
TIM_TypeDef * MB1_conf_miscTIM_p = TIM6;
ISRMgr_ns::ISR_t MB1_conf_miscTIM_ISRType = ISRMgr_ns::ISRMgr_TIM6;
const uint16_t MB1_conf_miscTIMPrescaler = 1;
const uint16_t MB1_conf_miscTIMReloadVal = 36000; // for 1 msec with PCLK1 = 36MHz

const uint16_t MB1_conf_ledBeat_period = 500; //in msec
Led *MB1_conf_ledBeat_p = &MB1_Led_red;
/**< for SysTick and led_beat */

/**< for USART1 */
/*
const uint32_t MB1_conf_USART1_buadrate = 9600;
const bool MB1_conf_USART1_retarget_isUsed = false;
const uint8_t MB1_conf_USART1_retarget = USART_stdStream_stdout;
*/
/**< for USART1 */

/**< for USART2 */
const bool MB1_USART2_isUsed = true;
const uint32_t MB1_conf_USART2_buadrate = 9600;
const bool MB1_conf_USART2_retarget_isUsed = false;
const uint8_t MB1_conf_USART2_retarget = USART_stdStream_stdout;
/**< for USART1 */

/**< for ISRs */
const bool MB1_conf_LedBeat_isUsed = true;
const bool MB1_conf_delayms_isUsed = true;
const bool MB1_conf_btnProcessing_isUsed = true;
/**< for ISRs */

/**< others */
const bool MB1_conf_bugsFix_isUsed = false;
const bool MB1_conf_NJTRST_isntUsed = true;
/**< others */

/**< conf interface (compile-time) */

/* static functions */
static void MB1_defaultState_reset(void);

/* MBoard-1 initialization */
void MB1_system_init (void){
    /**< others */
    if (MB1_conf_bugsFix_isUsed)
        bugs_fix ();
    if (MB1_conf_NJTRST_isntUsed){
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig (GPIO_Remap_SWJ_NoJTRST , ENABLE);
    }

    /**< end others */

    /**< SysTick and led beat */
    miscTIM_run (MB1_conf_miscTIM_p, MB1_conf_miscTIMPrescaler, MB1_conf_miscTIMReloadVal);
    LedBeat (MB1_conf_LedBeat_isUsed, MB1_conf_ledBeat_period, MB1_conf_ledBeat_p);

    /**< end SysTick and led beat */

    /**< USART1 */
    /* Don't use UART1 with RIOT
    MB1_USART1.Restart (MB1_conf_USART1_buadrate);
    if (MB1_conf_USART1_retarget_isUsed)
        MB1_USART1.Retarget (USART_stdStream_stdout);
    */

    /**< end USART1 */

    /**< USART2 */
    if (MB1_USART2_isUsed){
        MB1_USART2.Restart (MB1_conf_USART2_buadrate);
        if (MB1_conf_USART2_retarget_isUsed)
            MB1_USART2.Retarget (USART_stdStream_stdout);
    }

    /**< end USART2 */

    /**< ISRs */
    if (MB1_conf_LedBeat_isUsed)
        MB1_ISRs.subISR_assign (MB1_conf_miscTIM_ISRType, LedBeat_miscTIMISR);
    if (MB1_conf_delayms_isUsed)
        MB1_ISRs.subISR_assign (MB1_conf_miscTIM_ISRType, delay_ms_miscTIMISR);
    if (MB1_conf_btnProcessing_isUsed)
        MB1_ISRs.subISR_assign (MB1_conf_miscTIM_ISRType, btnProcessing_miscTIMISR);
    /**< end ISRs */

    /**< NVIC priority group config */
    NVIC_PriorityGroupConfig (MB1_NVIC_PriorityGroup);

    /**< Vector table relocation */
    if (MB1_VectorTableRelocation_isUsed){
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, MB1_VectorTableRelocationOffset);
    }

    /* Reset MBoard-1 system to default state */
    MB1_defaultState_reset();
 }

/**
 * @brief   Reset MBoard-1 system to default state.
 *          MB1_Led_red, green off.
 */
static void MB1_defaultState_reset(void) {
    MB1_Led_red.off();
    MB1_Led_green.off();
}
