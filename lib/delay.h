#ifndef _DELAY_H
#define _DELAY_H
#include "sys.h"

/*
 * delay_ms — when SYSTEM_SUPPORT_OS=1 and scheduler is running, uses
 * vTaskDelay() to yield CPU; busy-wait fallback before scheduler starts.
 * See delay.c for implementation.
 */
void delay_ms(u16 time);

#endif

