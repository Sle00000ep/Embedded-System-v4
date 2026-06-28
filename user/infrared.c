/**
 * infrared.c — HS0038 NEC infrared decoder (Exp17)
 *
 * EXTI12 falling-edge ISR → sets ir_received flag.
 * Main loop calls IR_Decode() which measures pulse widths with delay_us().
 *
 * NEC timing (20% tolerance):
 *   Leader:  9000us low, 4500us high (start) / 2250us high (repeat)
 *   Bit 0:    560us low,  560us high
 *   Bit 1:    560us low, 1690us high
 */

#include "infrared.h"
#include "pin_config.h"

volatile uint8_t ir_received = 0;

static uint8_t IR_ReadPin(void)
{
    return (GPIO_ReadInputDataBit(IR_PORT, IR_PIN) == Bit_RESET) ? 0 : 1;
}

/* Measure how many us pin stays at `level`; max_us is limit.
 * Uses DWT cycle counter for accurate timing, independent of loop speed. */
static uint32_t IR_PulseWidth(uint8_t level, uint32_t max_us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t max_cycles = max_us * (SystemCoreClock / 1000000U);
    while (IR_ReadPin() == level) {
        if ((DWT->CYCCNT - start) >= max_cycles) break;
    }
    return (DWT->CYCCNT - start) / (SystemCoreClock / 1000000U);
}

/* Check if val is within ±20% of nominal */
static uint8_t IR_InRange(uint32_t val, uint32_t nominal)
{
    uint32_t m = nominal / 5;
    return (val >= nominal - m && val <= nominal + m);
}

/* ---- Init ---- */
void IR_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(IR_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* PD12 = input pull-up (HS0038 open-collector, idle=high) */
    GPIO_InitStructure.GPIO_Pin  = IR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(IR_PORT, &GPIO_InitStructure);

    /* Map PD12 to EXTI12 */
    GPIO_EXTILineConfig(IR_EXTI_PORT, IR_EXTI_PIN);

    /* EXTI12 falling edge */
    EXTI_InitStructure.EXTI_Line    = IR_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* NVIC: EXTI15_10 shares IRQ */
    NVIC_InitStructure.NVIC_IRQChannel                   = IR_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void IR_EXTI_Enable(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = IR_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(IR_EXTI_LINE);
    ir_received = 0;
}

void IR_EXTI_Disable(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = IR_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(IR_EXTI_LINE);
}

/* ---- EXTI15_10 IRQ Handler ---- */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(IR_EXTI_LINE) != RESET) {
        ir_received = 1;
        EXTI_ClearITPendingBit(IR_EXTI_LINE);
    }
}

/**
 * Blocking NEC decode.  Call ONLY when ir_received==1 (pin already low).
 * Disables EXTI12 during measurement; caller must re-enable afterwards.
 */
IR_Data IR_Decode(void)
{
    IR_Data result = {0, 0, 0, 0};
    uint32_t low_time, high_time;
    uint32_t val = 0;
    uint8_t i;

    /* ---- Leader low (pin is already low from EXTI trigger) ---- */
    low_time = IR_PulseWidth(0, 11000);
    if (!IR_InRange(low_time, 9000)) return result;

    /* ---- Leader high ---- */
    high_time = IR_PulseWidth(1, 6000);

    /* Repeat code? */
    if (IR_InRange(high_time, 2250)) {
        /* Skip trailing 560us low if present */
        IR_PulseWidth(0, 1000);
        result.is_repeat = 1;
        result.valid = 1;
        return result;
    }

    if (!IR_InRange(high_time, 4500)) return result;

    /* ---- 32 bits ---- */
    for (i = 0; i < 32; i++) {
        low_time = IR_PulseWidth(0, 1000);
        if (!IR_InRange(low_time, 560)) return result;

        high_time = IR_PulseWidth(1, 2500);
        val >>= 1;
        if (IR_InRange(high_time, 1690)) {
            val |= 0x80000000UL;
        } else if (!IR_InRange(high_time, 560)) {
            return result;
        }
    }

    /* ---- Parse ---- */
    result.addr = (uint8_t)(val);
    result.cmd  = (uint8_t)(val >> 16);

    /* Validate complement fields */
    if ((uint8_t)(val >> 8)  == (uint8_t)(~result.addr) &&
        (uint8_t)(val >> 24) == (uint8_t)(~result.cmd)) {
        result.valid = 1;
    }

    return result;
}
