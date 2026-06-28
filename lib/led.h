#ifndef _LED_H
#define _LED_H
#include "sys.h"

// ============================================================
// V4板子: STM32F103ZET6, LED -> PF0-PF15
// ============================================================
#define LED_PORT   GPIOF
#define LED_RCC    RCC_APB2Periph_GPIOF
#define LEDout(i)  PFout(i)

#define led0 LEDout(0)
#define led1 LEDout(1)
#define led2 LEDout(2)
#define led3 LEDout(3)
#define led4 LEDout(4)
#define led5 LEDout(5)
#define led6 LEDout(6)
#define led7 LEDout(7)
#define led8 LEDout(8)
#define led9 LEDout(9)
#define led10 LEDout(10)
#define led11 LEDout(11)
#define led12 LEDout(12)
#define led13 LEDout(13)
#define led14 LEDout(14)
#define led15 LEDout(15)

void led_init(void);
#endif
