#include "sys.h"
#include "delay.h"

/*
 * When FreeRTOS is integrated (SYSTEM_SUPPORT_OS=1) and the scheduler is
 * running, delay_ms() yields the CPU via vTaskDelay() instead of busy-waiting.
 * Before the scheduler starts, the busy-wait fallback is used.
 *
 * scheduler_started flag is set by ControlTask (first RTOS task to run).
 */
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"
#include "task.h"
extern volatile uint8_t scheduler_started;
#endif

void delay_ms(u16 time)
{
#if SYSTEM_SUPPORT_OS
    if (scheduler_started) {
        vTaskDelay(pdMS_TO_TICKS(time));
        return;
    }
#endif
    /* Busy-wait fallback (used before scheduler starts) */
    u16 i = 0;
    while (time--) {
        i = 12000;
        while (i--);
    }
}
