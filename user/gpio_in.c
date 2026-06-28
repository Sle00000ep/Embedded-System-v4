/**
 * gpio_in.c — GPIO input driver
 *
 * IKEY0-3 (PF0-PF3): MainBoard independent keys, active-low, 10K pull-up
 *                     Share pins with LED1-4 — reconfigure direction per experiment
 * SW0-7  (PF4-PF11): MainBoard DIP switches, active-high, 10K pull-down
 *                     SW0-3 share pins with LED5-8
 *
 * EXTI ISRs for Exp8: light-weight, just set flag; main loop does debounce
 */

#include "gpio_in.h"
#include "pin_config.h"

/* ---- EXTI press flags (set by ISR, read & cleared by experiment) ---- */
volatile uint8_t ikey_pressed[4] = {0, 0, 0, 0};

/* ================================================================
 *  Polling mode — IKEY0-3 as GPIO_Mode_IPU (Exp2)
 *  Caller is responsible for reconfiguring PF0-PF3 direction.
 * ================================================================ */
void IKEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(IKEY0_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;   /* 10K pull-up, pressed = low */
    GPIO_InitStructure.GPIO_Pin   = IKEY0_PIN | IKEY1_PIN | IKEY2_PIN | IKEY3_PIN;
    GPIO_Init(IKEY0_PORT, &GPIO_InitStructure);
}

/**
 * Read single independent key (polling, no debounce)
 * idx: 0-3 (IKEY0-IKEY3)
 * returns: 1 = pressed (low), 0 = released (high)
 */
uint8_t IKEY_Read(uint8_t idx)
{
    uint16_t pin;
    switch (idx) {
        case 0: pin = IKEY0_PIN; break;
        case 1: pin = IKEY1_PIN; break;
        case 2: pin = IKEY2_PIN; break;
        case 3: pin = IKEY3_PIN; break;
        default: return 0;
    }
    return (GPIO_ReadInputDataBit(IKEY0_PORT, pin) == Bit_RESET) ? 1 : 0;
}

/* ================================================================
 *  EXTI interrupt mode — IKEY0-3 falling-edge (Exp8)
 * ================================================================ */
void IKEY_EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* GPIO clock + AFIO (for EXTI line mapping) */
    RCC_APB2PeriphClockCmd(IKEY0_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* PF0-PF3 as pull-up inputs */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = IKEY0_PIN | IKEY1_PIN | IKEY2_PIN | IKEY3_PIN;
    GPIO_Init(IKEY0_PORT, &GPIO_InitStructure);

    /* Map PF0-3 to EXTI0-3 */
    GPIO_EXTILineConfig(IKEY0_EXTI_PORT, IKEY0_EXTI_PIN);
    GPIO_EXTILineConfig(IKEY1_EXTI_PORT, IKEY1_EXTI_PIN);
    GPIO_EXTILineConfig(IKEY2_EXTI_PORT, IKEY2_EXTI_PIN);
    GPIO_EXTILineConfig(IKEY3_EXTI_PORT, IKEY3_EXTI_PIN);

    /* EXTI: falling edge (key press = high→low with pull-up) */
    EXTI_InitStructure.EXTI_Line    = IKEY0_EXTI_LINE | IKEY1_EXTI_LINE
                                    | IKEY2_EXTI_LINE | IKEY3_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* NVIC for EXTI0 (shares no IRQ with others) */
    NVIC_InitStructure.NVIC_IRQChannel                   = IKEY0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* EXTI1 */
    NVIC_InitStructure.NVIC_IRQChannel = IKEY1_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* EXTI2 */
    NVIC_InitStructure.NVIC_IRQChannel = IKEY2_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* EXTI3 */
    NVIC_InitStructure.NVIC_IRQChannel = IKEY3_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* Clear any pending flags from startup */
    EXTI_ClearITPendingBit(IKEY0_EXTI_LINE);
    EXTI_ClearITPendingBit(IKEY1_EXTI_LINE);
    EXTI_ClearITPendingBit(IKEY2_EXTI_LINE);
    EXTI_ClearITPendingBit(IKEY3_EXTI_LINE);

    /* Reset press flags */
    ikey_pressed[0] = ikey_pressed[1] = ikey_pressed[2] = ikey_pressed[3] = 0;
}

/**
 * Disable EXTI + NVIC for IKEY0-3, return pins to IPU (no interrupt)
 */
void IKEY_EXTI_DeInit(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Disable EXTI lines */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = IKEY0_EXTI_LINE | IKEY1_EXTI_LINE
                                    | IKEY2_EXTI_LINE | IKEY3_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Disable NVIC */
    NVIC_InitStructure.NVIC_IRQChannel                   = IKEY0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = IKEY1_IRQ;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = IKEY2_IRQ;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = IKEY3_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* Clear any pending */
    EXTI_ClearITPendingBit(IKEY0_EXTI_LINE);
    EXTI_ClearITPendingBit(IKEY1_EXTI_LINE);
    EXTI_ClearITPendingBit(IKEY2_EXTI_LINE);
    EXTI_ClearITPendingBit(IKEY3_EXTI_LINE);
}

/* ================================================================
 *  EXTI0-3 Interrupt Handlers
 *  Light-weight: set flag, clear pending. Main loop debounces.
 * ================================================================ */
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        ikey_pressed[0] = 1;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        ikey_pressed[1] = 1;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
        ikey_pressed[2] = 1;
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        ikey_pressed[3] = 1;
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

/* ================================================================
 *  DIP Switch (Exp9) — PF4-PF11, 10K pull-down, ON=high
 *  Caller is responsible for pin direction.
 * ================================================================ */
void SW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SW0_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;   /* 10K pull-down, ON = high */
    GPIO_InitStructure.GPIO_Pin   = SW_ALL_PINS;      /* PF4-PF11 */
    GPIO_Init(SW_ALL_PORT, &GPIO_InitStructure);
}

/**
 * Read all 8 DIP switches as a byte
 * returns: bit0=SW0 … bit7=SW7, 1=ON (high)
 */
uint8_t SW_ReadAll(void)
{
    uint8_t val = 0;
    uint16_t idr = GPIO_ReadInputData(SW_ALL_PORT);

    if (idr & SW0_PIN) val |= 0x01;
    if (idr & SW1_PIN) val |= 0x02;
    if (idr & SW2_PIN) val |= 0x04;
    if (idr & SW3_PIN) val |= 0x08;
    if (idr & SW4_PIN) val |= 0x10;
    if (idr & SW5_PIN) val |= 0x20;
    if (idr & SW6_PIN) val |= 0x40;
    if (idr & SW7_PIN) val |= 0x80;

    return val;
}

/**
 * Read single DIP switch
 * idx: 0-7
 * returns: 1=ON, 0=OFF
 */
uint8_t SW_Read(uint8_t idx)
{
    uint16_t pin;
    switch (idx) {
        case 0: pin = SW0_PIN; break;
        case 1: pin = SW1_PIN; break;
        case 2: pin = SW2_PIN; break;
        case 3: pin = SW3_PIN; break;
        case 4: pin = SW4_PIN; break;
        case 5: pin = SW5_PIN; break;
        case 6: pin = SW6_PIN; break;
        case 7: pin = SW7_PIN; break;
        default: return 0;
    }
    return (GPIO_ReadInputDataBit(SW_ALL_PORT, pin) != Bit_RESET) ? 1 : 0;
}
