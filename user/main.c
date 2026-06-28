/**
 * main.c — V4 CoreBoard + MainBoard 21-Function FreeRTOS Integration
 *
 * Stage 8 (Exp21): 2-task FreeRTOS architecture
 *   ControlTask     (prio 4): WKUP polling, mode switching via queue
 *   ExperimentTask  (prio 3): runs current experiment until stop_requested
 *
 * Each experiment function checks stop_requested instead of counting
 * WKUP long-press ticks.  delay_ms() auto-converts to vTaskDelay().
 */

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_out.h"
#include "gpio_in.h"
#include "usart_debug.h"
#include "rs232.h"
#include "rs485.h"
#include "sensor_adc.h"
#include "dht11.h"
#include "ds18b20.h"
#include "infrared.h"
#include "timer_pwm.h"
#include "encoder.h"
#include "delay_us.h"
#include "lcd12864.h"
#include "ch451.h"
#include "relay.h"
#include "tlc5615.h"
#include "stepper.h"
#include "pin_config.h"
#include "freertos_tasks.h"

/* ============================================================
 *  WKUP Key (PA0) — used by ControlTask via extern
 * ============================================================ */
uint8_t WKUP_IsPressed(void)
{
    return (GPIO_ReadInputDataBit(WKUP_PORT, WKUP_PIN) == Bit_RESET) ? 1 : 0;
}

void WKUP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(WKUP_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = WKUP_PIN;
    GPIO_Init(WKUP_PORT, &GPIO_InitStructure);
}

/* ============================================================
 *  Exp1: LED Flowing  (PF0-PF7, 200ms/step)
 * ============================================================ */
void Exp1_LED_Flowing(void)
{
    uint8_t i = 0;
    LED_Init();
    printf("\r\n===== Exp1: LED Flowing =====\r\n");
    while (1) {
        if (stop_requested) { LED_AllOff(); return; }
        LED_AllOff();
        LED_Set(i, 1);
        i = (i + 1) % 8;
        delay_ms(200);
    }
}

/* ============================================================
 *  Exp5: Traffic Light  (PE0-PE5 + PE6 buzzer)
 * ============================================================ */
typedef enum { TL_NS_G_WE_R, TL_NS_Y_WE_R, TL_NS_R_WE_G, TL_NS_R_WE_Y } TrafficState;
void Exp5_TrafficLight(void)
{
    TrafficState s = TL_NS_G_WE_R;
    uint32_t ticks = 0, dur = 30;
    printf("\r\n===== Exp5: Traffic Light =====\r\n");
    TrafficLight_Set(2, 0); Buzzer_Set(0);
    while (1) {
        if (stop_requested) { TrafficLight_Set(0,0); Buzzer_Set(0); return; }
        ticks++;
        switch (s) {
            case TL_NS_G_WE_R: if (ticks>=dur){s=TL_NS_Y_WE_R;dur=10;ticks=0;TrafficLight_Set(1,0);printf("NS:Yel WE:Red\r\n");} break;
            case TL_NS_Y_WE_R: if (ticks>=dur){s=TL_NS_R_WE_G;dur=30;ticks=0;TrafficLight_Set(0,2);printf("NS:Red WE:Grn\r\n");} break;
            case TL_NS_R_WE_G: if (ticks>=dur){s=TL_NS_R_WE_Y;dur=10;ticks=0;TrafficLight_Set(0,1);printf("NS:Red WE:Yel\r\n");} break;
            case TL_NS_R_WE_Y: if (ticks>=dur){s=TL_NS_G_WE_R;dur=30;ticks=0;TrafficLight_Set(2,0);printf("NS:Grn WE:Red\r\n");Buzzer_Set(1);delay_ms(100);Buzzer_Set(0);} break;
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp2: Key Polling  (IKEY0-3 → LED5-8)
 * ============================================================ */
void Exp2_KeyPolling(void)
{
    uint8_t ks[4]={0}, ls[4]={0};
    uint8_t i;
    LED_Init(); IKEY_Init();
    printf("\r\n===== Exp2: Key Polling =====\r\n");
    while (1) {
        if (stop_requested) { LED_AllOff(); return; }
        for (i=0;i<4;i++) ks[i]=IKEY_Read(i);
        for (i=0;i<4;i++) {
            if (ks[i]!=ls[i]) { ls[i]=ks[i]; LED_Set(i+4,ks[i]); printf("IKEY%d %s\r\n",i,ks[i]?"PRESSED":"released"); }
        }
        delay_ms(50);
    }
}

/* ============================================================
 *  Exp8: Independent Keys EXTI  (IKEY0-3 EXTI → toggle LED5-8)
 * ============================================================ */
void Exp8_IndependentKeys(void)
{
    uint8_t led_t[4]={0};
    uint8_t i;
    LED_Init(); IKEY_EXTI_Init();
    printf("\r\n===== Exp8: Independent Keys (EXTI) =====\r\n");
    while (1) {
        if (stop_requested) { LED_AllOff(); IKEY_EXTI_DeInit(); return; }
        for (i=0;i<4;i++) {
            if (ikey_pressed[i]) { ikey_pressed[i]=0; delay_ms(10); if (IKEY_Read(i)) { led_t[i]=!led_t[i]; LED_Set(i+4,led_t[i]); printf("IKEY%d -> LED%d %s\r\n",i,i+5,led_t[i]?"ON":"OFF"); } }
        }
        delay_ms(20);
    }
}

/* ============================================================
 *  Exp9: DIP Switch  (SW0-7 → LED1-4 + serial)
 * ============================================================ */
void Exp9_DIPSwitch(void)
{
    uint8_t sw, last=0xFF;
    uint8_t i;
    LED_Init(); SW_Init();
    printf("\r\n===== Exp9: DIP Switch =====\r\n");
    while (1) {
        if (stop_requested) { LED_AllOff(); return; }
        sw = SW_ReadAll();
        if (sw != last) { last=sw; for(i=0;i<4;i++) LED_Set(i,(sw>>i)&1); printf("DIP:%d%d%d%d%d%d%d%d 0x%02X\r\n",(sw>>7)&1,(sw>>6)&1,(sw>>5)&1,(sw>>4)&1,(sw>>3)&1,(sw>>2)&1,(sw>>1)&1,sw&1,sw); }
        delay_ms(200);
    }
}

/* ============================================================
 *  Exp4: USART1 Serial Echo
 * ============================================================ */
void Exp4_USART1_Echo(void)
{
    uint8_t ch;
    USART_RX_Init(); USART_RX_Flush();
    printf("\r\n===== Exp4: USART1 Serial Echo =====\r\n");
    while (1) {
        if (stop_requested) { USART_RX_DeInit(); return; }
        while (USART_RX_Available()) { ch=USART_RX_Read(); printf("Echo:'%c' 0x%02X\r\n",(ch>=32&&ch<127)?ch:'.',ch); }
        delay_ms(20);
    }
}

/* ============================================================
 *  Exp15: RS232 (USART3)
 * ============================================================ */
void Exp15_RS232(void)
{
    uint32_t tick=0;
    uint8_t ch;
    RS232_Init(115200);
    printf("\r\n===== Exp15: RS232 (USART3) =====\r\n");
    RS232_SendString("RS232 Test Start\r\n");
    while (1) {
        if (stop_requested) { USART_Cmd(RS232_USART,DISABLE); return; }
        if (++tick % 20 == 0) { RS232_SendString("RS232 ping\r\n"); printf("[RS232] Sent:ping\r\n"); }
        while (RS232_RxAvailable()) { ch=RS232_ReadByte(); printf("[RS232] RX:'%c' 0x%02X\r\n",(ch>=32&&ch<127)?ch:'.',ch); }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp16: RS485 (USART4 half-duplex)
 * ============================================================ */
void Exp16_RS485(void)
{
    uint32_t tick=0;
    uint8_t ch;
    RS485_Init(115200);
    printf("\r\n===== Exp16: RS485 (USART4) =====\r\n");
    RS485_SendString("RS485 Test Start\r\n"); printf("[RS485] Sent first msg\r\n");
    while (1) {
        if (stop_requested) { USART_Cmd(RS485_USART,DISABLE); return; }
        if (++tick % 20 == 0) { RS485_SendString("RS485 ping\r\n"); printf("[RS485] Sent:ping\r\n"); }
        while (RS485_RxAvailable()) { ch=RS485_ReadByte(); printf("[RS485] RX:'%c' 0x%02X\r\n",(ch>=32&&ch<127)?ch:'.',ch); }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp3: Light Sensor ADC  (PA1 → LED bar + serial)
 * ============================================================ */
void Exp3_LightSensor(void)
{
    uint8_t pct, last_pct = 0xFF, i, n;
    LED_Init(); ADC1_Init();
    printf("\r\n===== Exp3: Light Sensor (ADC1 PA1) =====\r\n");
    printf("LED1-8 = brightness bar. Cover photoresistor to see change.\r\n");
    while (1) {
        if (stop_requested) { LED_AllOff(); return; }
        pct = Light_Percent();
        if (pct != last_pct) {
            last_pct = pct;
            printf("Light: %d%% (ADC=%d)\r\n", pct, ADC1_ReadAvg(ADC_LIGHT_CH,4));
            n = pct / 13;
            for (i=0;i<8;i++) LED_Set(i, (i<=n)?1:0);
        }
        delay_ms(200);
    }
}

/* ============================================================
 *  Exp11: DHT11 Temperature & Humidity  (PF12 One-Wire)
 * ============================================================ */
void Exp11_DHT11(void)
{
    uint8_t tick;
    DHT11_Init();
    printf("\r\n===== Exp11: DHT11 Temp & Humidity =====\r\n");
    printf("PF12 One-Wire, read interval ~2s\r\n");
    while (1) {
        for (tick = 0; tick < 40; tick++) {
            if (stop_requested) return;
            delay_ms(50);
        }
        DHT11_Data d = DHT11_Read();
        if (d.status == DHT11_OK)
            printf("Humidity: %d%%  Temperature: %d C\r\n", d.humidity, d.temperature);
        else if (d.status == DHT11_CHKSUM)
            printf("DHT11 checksum error\r\n");
        else
            printf("DHT11 timeout (check wiring / pull-up)\r\n");
    }
}

/* ============================================================
 *  Exp12: Thermistor ADC  (PC1 → serial)
 * ============================================================ */
void Exp12_Thermistor(void)
{
    uint16_t val, last_val = 0xFFFF;
    ADC1_Init();
    printf("\r\n===== Exp12: Thermistor ADC (PC1) =====\r\n");
    printf("PC1 = ADC123_IN11.  NTC voltage divider.\r\n");
    printf("Raw ADC value (0=0V, 4095=3.3V).\r\n");
    while (1) {
        if (stop_requested) return;
        val = ADC1_ReadAvg(ADC_TEMP_CH, 8);
        if (val != last_val) {
            last_val = val;
            printf("Thermistor: %d (%.3fV)\r\n", val, val * 3.3f / 4095.0f);
        }
        delay_ms(200);
    }
}

/* ============================================================
 *  Exp13: DS18B20 Temperature  (PF13 One-Wire)
 * ============================================================ */
void Exp13_DS18B20(void)
{
    uint8_t tick;
    DS18B20_Init();
    printf("\r\n===== Exp13: DS18B20 Temperature =====\r\n");
    printf("PF13 One-Wire.  2s cycle.\r\n");
    while (1) {
        for (tick = 0; tick < 40; tick++) {
            if (stop_requested) return;
            delay_ms(50);
        }
        DS18B20_StartConvert();
        for (tick = 0; tick < 16; tick++) {
            if (stop_requested) return;
            delay_ms(50);
        }
        DS18B20_Data d = DS18B20_ReadTemp();
        if (d.status == DS18B20_OK) {
            int t = d.temp_x10;
            printf("Temp: %c%d.%d C\r\n", (t<0)?'-':' ', (t<0?-t:t)/10, (t<0?-t:t)%10);
        } else {
            printf("DS18B20 not found (check wiring / pull-up)\r\n");
        }
    }
}

/* ============================================================
 *  Exp17: Infrared Receiver  (PD12, HS0038, NEC decode)
 * ============================================================ */
void Exp17_Infrared(void)
{
    IR_Init();
    printf("\r\n===== Exp17: Infrared Receiver =====\r\n");
    printf("PD12 -> HS0038.  Point remote and press buttons.\r\n");
    printf("Long-press WKUP (>500ms) to switch\r\n\r\n");
    while (1) {
        if (stop_requested) { IR_EXTI_Disable(); return; }

        if (ir_received) {
            ir_received = 0;
            IR_EXTI_Disable();
            IR_Data ir = IR_Decode();
            IR_EXTI_Enable();
            if (ir.valid) {
                if (ir.is_repeat) printf("IR: Repeat\r\n");
                else printf("IR: Addr=0x%02X  Cmd=0x%02X\r\n", ir.addr, ir.cmd);
            }
        }
        delay_ms(1);
    }
}

/* ============================================================
 *  Exp18: Rotary Encoder  (PB6=A PB7=B PG8=Z)
 * ============================================================ */
void Exp18_Encoder(void)
{
    int32_t count, last_count = 0;
    ENC_Init();
    printf("\r\n===== Exp18: Rotary Encoder =====\r\n");
    printf("PB6=A(P65) PB7=B(P66) PG8=Z(P23)\r\n");
    printf("Rotate encoder, count updates in real time.\r\n");
    while (1) {
        if (stop_requested) return;
        count = ENC_GetCount();
        if (count != last_count) {
            last_count = count;
            printf("Encoder: %ld\r\n", (long)count);
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp19: DC Motor PWM  (PB0=PWMA=speed, PB1=direction)
 * ============================================================ */
void Exp19_DCMotorPWM(void)
{
    uint32_t tick = 0;
    uint8_t state = 0;

    Motor_Init();
    Motor_SetDir(0);
    Motor_SetSpeed(50);
    printf("\r\n===== Exp19: DC Motor PWM =====\r\n");
    printf("PB0=PWMA(P102)=Speed PWM  PB1=DIR(P101)\r\n");
    printf("L9110S: IA=PWM IB=0=forward, IA=0 IB=PWM=reverse\r\n");
    printf("Auto: FWD50%% -> FWD100%% -> REV50%% -> REV100%% every 3s\r\n");

    while (1) {
        if (stop_requested) { Motor_Stop(); return; }
        if (++tick % 30 == 0) {
            state = (state + 1) % 4;
            switch (state) {
                case 0:
                    Motor_SetDir(0); Motor_SetSpeed(50);
                    printf("Motor: Forward 50%%\r\n");
                    break;
                case 1:
                    Motor_SetDir(0); Motor_SetSpeed(100);
                    printf("Motor: Forward 100%%\r\n");
                    break;
                case 2:
                    Motor_SetDir(1); Motor_SetSpeed(50);
                    printf("Motor: Reverse 50%%\r\n");
                    break;
                case 3:
                    Motor_SetDir(1); Motor_SetSpeed(100);
                    printf("Motor: Reverse 100%%\r\n");
                    break;
            }
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp20: Servo  (PB9=TIM4_CH4, 50Hz)
 * ============================================================ */
void Exp20_Servo(void)
{
    uint32_t tick = 0;
    uint8_t ang_idx = 0;
    const uint8_t angles[3] = {0, 90, 180};
    TIM4_Servo_Init();
    TIM4_Servo_SetAngle(90);
    printf("\r\n===== Exp20: Servo =====\r\n");
    printf("PB9=SERVO(P69) TIM4_CH4 50Hz\r\n");
    printf("Angle auto-cycle: 0->90->180 every 3s.\r\n");
    while (1) {
        if (stop_requested) { TIM4_Servo_SetAngle(90); return; }
        if (++tick % 30 == 0) {
            uint8_t ang = angles[ang_idx];
            ang_idx = (ang_idx + 1) % 3;
            TIM4_Servo_SetAngle(ang);
            printf("Angle: %d deg\r\n", ang);
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp6: LCD 12864 Serial Display  (PE7-PE11 ST7920)
 * ============================================================ */
void Exp6_LCD12864(void)
{
    uint32_t tick = 0;
    uint8_t page = 0;
    LCD12864_Init();
    LCD12864_Clear();
    printf("\r\n===== Exp6: LCD 12864 =====\r\n");
    printf("PE7=CS PE8=STD PE9=SCLK PE10=BLA PE11=RST\r\n");
    printf("ST7920 serial mode. Auto-cycle display pages.\r\n");
    while (1) {
        if (stop_requested) {
            LCD12864_Clear();
            LCD12864_Backlight(0);
            return;
        }

        if (++tick % 30 == 0) {
            page = (page + 1) % 4;
            LCD12864_Clear();

            switch (page) {
                case 0:
                    LCD12864_SetPos(0, 0);
                    LCD12864_WriteString("STM32F1");
                    LCD12864_SetPos(1, 0);
                    LCD12864_WriteString("LCD 12864");
                    LCD12864_SetPos(2, 0);
                    LCD12864_WriteString("Serial");
                    LCD12864_SetPos(3, 0);
                    LCD12864_WriteString("Mode OK!");
                    printf("[LCD] Page 0: Welcome\r\n");
                    break;
                case 1: {
                    LCD12864_SetPos(0, 0);
                    LCD12864_WriteString("Counter:");
                    LCD12864_SetPos(2, 0);
                    LCD12864_WriteString("Val=");
                    LCD12864_DisplayNum(tick);
                    printf("[LCD] Page 1: Counter %lu\r\n", (unsigned long)tick);
                    break;
                }
                case 2:
                    LCD12864_SetPos(0, 0);
                    LCD12864_WriteString("GPIO: PE7");
                    LCD12864_SetPos(1, 0);
                    LCD12864_WriteString("-PE11");
                    LCD12864_SetPos(2, 0);
                    LCD12864_WriteString("CS/STD/CLK");
                    LCD12864_SetPos(3, 0);
                    LCD12864_WriteString("BLA/RST");
                    printf("[LCD] Page 2: Pin info\r\n");
                    break;
                case 3:
                    LCD12864_SetPos(0, 0);
                    LCD12864_WriteString("ADC=");
                    LCD12864_DisplayFloat(3.30f, 2);
                    LCD12864_SetPos(2, 0);
                    LCD12864_WriteString("Freq=72M");
                    LCD12864_SetPos(3, 0);
                    LCD12864_WriteString("Hz OK");
                    printf("[LCD] Page 3: System info\r\n");
                    break;
            }
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp7: CH451 7-Seg Display + Keyboard Scan  (PD8-PD11)
 * ============================================================ */
void Exp7_CH451(void)
{
    uint32_t tick = 0;
    int32_t counter = 0;
    uint8_t key;

    CH451_Init();
    printf("\r\n===== Exp7: CH451 7-Seg + Keyboard =====\r\n");
    printf("PD8=DCLK PD9=DIN PD10=DOUT PD11=LOAD\r\n");
    printf("8-digit 7-seg display. Press matrix keys.\r\n");
    printf("Counter auto-increments. Keys shown on serial.\r\n");

    CH451_DisplayNum(counter);

    while (1) {
        if (stop_requested) {
            CH451_Clear();
            return;
        }

        /* Poll keyboard */
        key = CH451_ReadKey();
        if (key != 0xFF) {
            printf("[CH451] Key: %d (0x%02X)\r\n", key, key);
            CH451_DisplayNum(key);
            delay_ms(500);
            CH451_DisplayNum(counter);
        }

        if (++tick % 5 == 0) {
            counter = (counter + 1) % 10000;
            CH451_DisplayNum(counter);
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp10: Relay  (PG2=RELAY1, PG3=RELAY2)
 * ============================================================ */
void Exp10_Relay(void)
{
    uint32_t tick = 0;
    uint8_t state = 0;

    LED_Init();
    Relay_Init();
    printf("\r\n===== Exp10: Relay =====\r\n");
    printf("PG2=RELAY1(P17) PG3=RELAY2(P16)\r\n");
    printf("LED1=RELAY1  LED2=RELAY2  (core board LEDs)\r\n");
    printf("Auto-cycle: OFF->R1->R2->BOTH every 2s\r\n");

    while (1) {
        if (stop_requested) {
            Relay_Set(1, 0); Relay_Set(2, 0);
            LED_AllOff();
            return;
        }

        if (++tick % 20 == 0) {
            state = (state + 1) % 4;
            switch (state) {
                case 0:
                    Relay_Set(1, 0); Relay_Set(2, 0);
                    LED_Set(0, 0); LED_Set(1, 0);
                    printf("Relay: Both OFF\r\n");
                    break;
                case 1:
                    Relay_Set(1, 1); Relay_Set(2, 0);
                    LED_Set(0, 1); LED_Set(1, 0);
                    printf("Relay: R1 ON, R2 OFF\r\n");
                    break;
                case 2:
                    Relay_Set(1, 0); Relay_Set(2, 1);
                    LED_Set(0, 0); LED_Set(1, 1);
                    printf("Relay: R1 OFF, R2 ON\r\n");
                    break;
                case 3:
                    Relay_Set(1, 1); Relay_Set(2, 1);
                    LED_Set(0, 1); LED_Set(1, 1);
                    printf("Relay: Both ON\r\n");
                    break;
            }
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp14: TLC5615 DA Output  (GPIO bit-bang, PB12/PB13/PB15)
 * ============================================================ */
void Exp14_DA(void)
{
    uint32_t tick = 0;
    uint8_t step = 0;
    const uint16_t voltages[3] = {0, 1650, 3300};

    TLC5615_Init();
    printf("\r\n===== Exp14: TLC5615 DA Output =====\r\n");
    printf("PB12=CS(P3) PB13=SCLK(P2) PB15=DIN(P4)\r\n");
    printf("Fixed voltages: 0V -> 1.65V -> 3.3V, 3s each\r\n");
    printf("Measure DA-OUT pin with multimeter DC mode.\r\n");

    while (1) {
        if (stop_requested) {
            TLC5615_SetValue(0);
            return;
        }

        if (++tick % 30 == 0) {
            uint16_t mv = voltages[step];
            step = (step + 1) % 3;
            TLC5615_SetVoltage(mv);
            printf("DA set %d mV (code=%d) -- measure OUT now\r\n",
                   mv, (int)((uint32_t)mv * 1024 / 6600));
        }
        delay_ms(100);
    }
}

/* ============================================================
 *  Exp18: Stepper Motor  (PE12-PE15 + Encoder PB6/PB7/PG8)
 * ============================================================ */
void Exp18_StepperMotor(void)
{
    uint32_t tick = 0;
    int32_t count, last_count = 0;
    uint8_t dir = 0;

    Stepper_Init();
    ENC_Init();
    printf("\r\n===== Exp18: Stepper Motor =====\r\n");
    printf("PE12=A(P83) PE13=B(P82) PE14=C(P81) PE15=D(P80)\r\n");
    printf("Encoder: PB6(P65) PB7(P66) PG8(P23)\r\n");
    printf("Auto: CW 256 steps -> pause -> CCW 256 steps -> repeat\r\n");

    while (1) {
        if (stop_requested) {
            Stepper_Stop();
            return;
        }

        if (++tick % 30 == 0) {
            if (dir == 0) {
                printf("Stepper: CW 256 steps...\r\n");
                Stepper_StepCW(256);
            } else {
                printf("Stepper: CCW 256 steps...\r\n");
                Stepper_StepCCW(256);
            }
            dir = !dir;

            count = ENC_GetCount();
            printf("Encoder: %ld\r\n", (long)count);
        }

        count = ENC_GetCount();
        if (count != last_count) {
            last_count = count;
            printf("Enc: %ld\r\n", (long)count);
        }

        delay_ms(100);
    }
}

/* ============================================================
 *  main  — hardware init → create RTOS objects → start scheduler
 * ============================================================ */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    delay_us_init();

    LED_Init();
    TrafficLight_Init();
    WKUP_Init();
    usart_init(115200);

    printf("\r\n=================================\r\n");
    printf("  V4 CoreBoard + MainBoard\r\n");
    printf("  21-Function + FreeRTOS V9.0.0\r\n");
    printf("=================================\r\n");
    printf("2 Tasks: Control(pri4) + Experiment(pri3)\r\n");
    printf("Long-press WKUP (>500ms) to cycle modes.\r\n\r\n");

    freertos_init();

    vTaskStartScheduler();

    /* Should never reach here */
    while (1);
}
