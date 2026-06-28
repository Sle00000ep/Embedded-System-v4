/**
 * timer_pwm.c — DC Motor PWM + Servo (Exp19, Exp20)
 *
 * Motor (L9110S H-bridge):
 *   PB0 = TIM3_CH3 = PWM speed  (1kHz, 0-100% duty)
 *   PB1 = GPIO OUT  = direction (LOW=forward, HIGH=reverse)
 *
 *   L9110S truth table:
 *     IA=PWM  IB=0  →  forward
 *     IA=0    IB=PWM → reverse
 *     IA=0    IB=0  →  stop (coast)
 *
 * Servo: PB9 (TIM4_CH4), 50Hz, 0°=500us, 180°=2500us
 */

#include "timer_pwm.h"
#include "pin_config.h"

/* ================================================================
 *  DC Motor — PB0=PWM speed, PB1=GPIO direction
 * ================================================================ */
void Motor_Init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(PWMA_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(TIM3_SYS_CLK, ENABLE);

    /* PB0 = alternate-function push-pull (TIM3_CH3 PWM) */
    GPIO_InitStructure.GPIO_Pin   = PWMA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(PWMA_PORT, &GPIO_InitStructure);

    /* PB1 = push-pull output (direction) */
    GPIO_InitStructure.GPIO_Pin   = PWMB_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(PWMB_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(PWMB_PORT, PWMB_PIN);   /* default LOW = forward */

    /* TIM3: 1kHz PWM (72MHz / 72 = 1MHz, 1MHz / 1000 = 1kHz) */
    TIM_TimeBaseStructure.TIM_Period        = 999;
    TIM_TimeBaseStructure.TIM_Prescaler     = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* CH3 (PB0): PWM1, 0% duty initially (stop) */
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;        /* 0% = stop */
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * Set motor speed 0-100%.
 *   pct = 0   → stop
 *   pct = 100 → full speed
 */
void Motor_SetSpeed(uint8_t pct)
{
    uint16_t ccr;
    if (pct > 100) pct = 100;

    /* CCR = pct% of period (0-999) */
    ccr = (uint16_t)(((uint32_t)pct * 999) / 100);
    TIM_SetCompare3(TIM3, ccr);
}

/**
 * Set motor direction.
 *   dir = 0 → forward  (PB1=LOW)
 *   dir = 1 → reverse  (PB1=HIGH)
 */
void Motor_SetDir(uint8_t dir)
{
    if (dir)
        GPIO_SetBits(PWMB_PORT, PWMB_PIN);    /* PB1=HIGH → reverse */
    else
        GPIO_ResetBits(PWMB_PORT, PWMB_PIN);  /* PB1=LOW  → forward */
}

void Motor_Stop(void)
{
    TIM_SetCompare3(TIM3, 0);                  /* 0% duty */
    GPIO_ResetBits(PWMB_PORT, PWMB_PIN);       /* PB1=LOW */
}

/* ================================================================
 *  TIM4 Servo — PB9 (TIM4_CH4), 50Hz, 0°=500us, 180°=2500us
 * ================================================================ */
void TIM4_Servo_Init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(SERVO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(TIM4_SYS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = SERVO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period        = 19999;
    TIM_TimeBaseStructure.TIM_Prescaler     = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 1500;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);

    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;
    if (angle > 180) angle = 180;
    pulse = 500 + ((uint32_t)angle * 2000) / 180;
    TIM_SetCompare4(TIM4, pulse);
}
