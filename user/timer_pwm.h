/**
 * timer_pwm.h — TIM3 PWM (DC motor) + TIM4 Servo header (Exp19, Exp20)
 *
 * TIM3_CH3 (PB0): DC motor speed PWM, 8kHz
 * PB1 (PWMB):    DC motor direction GPIO  (1=forward, 0=reverse)
 * TIM4_CH4 (PB9): Servo 50Hz PWM, 0.5-2.5ms pulse
 */

#ifndef __TIMER_PWM_H
#define __TIMER_PWM_H

#include "stm32f10x.h"

/* ---- DC Motor (PB0=PWM speed, PB1=direction) ---- */
void Motor_Init(void);
void Motor_SetSpeed(uint8_t pct);   /* 0-100% duty on PB0 */
void Motor_SetDir(uint8_t dir);     /* 1=forward, 0=reverse */
void Motor_Stop(void);               /* speed=0 */

/* ---- Servo (TIM4 CH4, PB9) ---- */
void TIM4_Servo_Init(void);
void TIM4_Servo_SetAngle(uint8_t angle); /* 0-180 degrees */

#endif /* __TIMER_PWM_H */
