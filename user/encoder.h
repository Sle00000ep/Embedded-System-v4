/**
 * encoder.h — Rotary encoder counter header (Exp18)
 *
 * PB6 = A phase (EXTI6 falling edge → count ±1, direction from PB7)
 * PB7 = B phase (GPIO input: high=CW, low=CCW)
 * PG8 = Z phase (EXTI8 falling edge → reset count to 0)
 */

#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

extern volatile int32_t enc_count;

void ENC_Init(void);
int32_t ENC_GetCount(void);
void    ENC_Reset(void);

#endif /* __ENCODER_H */
