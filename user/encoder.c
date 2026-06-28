/**
 * encoder.c — Rotary encoder counter (Exp18)
 *
 * PB6 (A): EXTI6 falling edge → ISR reads PB7 direction → enc_count ±= 1
 * PB7 (B): GPIO_Mode_IPU, read in ISR to determine direction
 * PG8 (Z): EXTI8 falling edge → ISR resets enc_count = 0
 *
 * EXTI6 + EXTI8 share IRQ (EXTI9_5_IRQn), handled in single ISR below.
 */

#include "encoder.h"
#include "pin_config.h"

volatile int32_t enc_count = 0;

void ENC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* Clocks */
    RCC_APB2PeriphClockCmd(ENC_A_CLK, ENABLE);   /* GPIOB */
    RCC_APB2PeriphClockCmd(ENC_Z_CLK, ENABLE);   /* GPIOG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* PB6 = A phase, input pull-up */
    GPIO_InitStructure.GPIO_Pin  = ENC_A_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(ENC_A_PORT, &GPIO_InitStructure);

    /* PB7 = B phase, input pull-up (read as GPIO for direction) */
    GPIO_InitStructure.GPIO_Pin  = ENC_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(ENC_B_PORT, &GPIO_InitStructure);

    /* PG8 = Z phase, input pull-up */
    GPIO_InitStructure.GPIO_Pin  = ENC_Z_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(ENC_Z_PORT, &GPIO_InitStructure);

    /* Map PB6 → EXTI6, PG8 → EXTI8 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource8);

    /* EXTI6 + EXTI8: falling edge */
    EXTI_InitStructure.EXTI_Line    = EXTI_Line6 | EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* NVIC: EXTI9_5 shares IRQ for lines 5-9 */
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    enc_count = 0;
}

int32_t ENC_GetCount(void)
{
    return enc_count;
}

void ENC_Reset(void)
{
    enc_count = 0;
}

/**
 * EXTI9_5 interrupt handler — lines 5 through 9
 * ENC_A (EXTI6): pulse count ± 1, direction from PB7
 * ENC_Z (EXTI8): reset count to 0
 */
void EXTI9_5_IRQHandler(void)
{
    /* ENC_A — PB6 falling edge */
    if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
        /* Read PB7 to determine direction */
        if (GPIO_ReadInputDataBit(ENC_B_PORT, ENC_B_PIN) != Bit_RESET)
            enc_count++;   /* PB7 high → CW */
        else
            enc_count--;   /* PB7 low  → CCW */
        EXTI_ClearITPendingBit(EXTI_Line6);
    }

    /* ENC_Z — PG8 falling edge → reset */
    if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
        enc_count = 0;
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}
