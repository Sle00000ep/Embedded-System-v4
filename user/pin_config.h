/**
 * pin_config.h — V4核心板 + MainBoard试验箱 21功能引脚宏定义
 *
 * 所有引脚引用集中在此文件，代码中统一使用宏名，不直接写 GPIO_Pin_X。
 * 映射依据：V4核心板_STM32F103ZET6_引脚定义.md (100%确认)
 */

#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

#include "stm32f10x.h"

/* ================================================================
 *  核心板开关配置
 *  SW0=ON, SW1=UART, SW2=OFF(Flash), SW3=ON(给PF LED供电), AD=ON
 * ================================================================ */

/* ================================================================
 *  LED1-LED8 (实验1) — GPIOF PF0-PF7 核心板板载LED 低电平亮
 *  SW3 须设为 ON（给PF LED供电）
 *  PF0-PF3 与 MainBoard 按键 IKEY0-3 复用，运行时按实验模式切换方向
 * ================================================================ */
#define LED1_PORT       GPIOF
#define LED1_PIN        GPIO_Pin_0
#define LED1_CLK        RCC_APB2Periph_GPIOF

#define LED2_PORT       GPIOF
#define LED2_PIN        GPIO_Pin_1
#define LED2_CLK        RCC_APB2Periph_GPIOF

#define LED3_PORT       GPIOF
#define LED3_PIN        GPIO_Pin_2
#define LED3_CLK        RCC_APB2Periph_GPIOF

#define LED4_PORT       GPIOF
#define LED4_PIN        GPIO_Pin_3
#define LED4_CLK        RCC_APB2Periph_GPIOF

#define LED5_PORT       GPIOF
#define LED5_PIN        GPIO_Pin_4
#define LED5_CLK        RCC_APB2Periph_GPIOF

#define LED6_PORT       GPIOF
#define LED6_PIN        GPIO_Pin_5
#define LED6_CLK        RCC_APB2Periph_GPIOF

#define LED7_PORT       GPIOF
#define LED7_PIN        GPIO_Pin_6
#define LED7_CLK        RCC_APB2Periph_GPIOF

#define LED8_PORT       GPIOF
#define LED8_PIN        GPIO_Pin_7
#define LED8_CLK        RCC_APB2Periph_GPIOF

/* 8路LED整体操作 */
#define LED_ALL_PORT    GPIOF
#define LED_ALL_PINS    (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                         GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)

/* ================================================================
 *  交通灯 (实验5) — GPIOE PE0-PE5 MainBoard 低电平亮
 *  NS=North-South, WE=West-East
 *  需飞线: P70=NS-R P71=NS-Y P146=NS-G P147=WE-R P144=WE-Y P145=WE-G
 * ================================================================ */
#define NS_R_PORT       GPIOE
#define NS_R_PIN        GPIO_Pin_0
#define NS_R_CLK        RCC_APB2Periph_GPIOE

#define NS_Y_PORT       GPIOE
#define NS_Y_PIN        GPIO_Pin_1
#define NS_Y_CLK        RCC_APB2Periph_GPIOE

#define NS_G_PORT       GPIOE
#define NS_G_PIN        GPIO_Pin_2
#define NS_G_CLK        RCC_APB2Periph_GPIOE

#define WE_R_PORT       GPIOE
#define WE_R_PIN        GPIO_Pin_3
#define WE_R_CLK        RCC_APB2Periph_GPIOE

#define WE_Y_PORT       GPIOE
#define WE_Y_PIN        GPIO_Pin_4
#define WE_Y_CLK        RCC_APB2Periph_GPIOE

#define WE_G_PORT       GPIOE
#define WE_G_PIN        GPIO_Pin_5
#define WE_G_CLK        RCC_APB2Periph_GPIOE

/* 6路交通灯整体操作 */
#define TRAFFIC_PORT    GPIOE
#define TRAFFIC_PINS    (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | \
                         GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5)

/* ================================================================
 *  蜂鸣器 (实验5) — GPIOE PE6 MainBoard 高电平响 (8050驱动)
 *  需飞线: P142=BUZZER
 * ================================================================ */
#define BUZZER_PORT     GPIOE
#define BUZZER_PIN      GPIO_Pin_6
#define BUZZER_CLK      RCC_APB2Periph_GPIOE

/* ================================================================
 *  LCD 12864 串行模式 (实验6) — GPIOE
 *  PSB 硬件接地 → 串口模式
 * ================================================================ */
#define LCD_CS_PORT     GPIOE
#define LCD_CS_PIN      GPIO_Pin_7
#define LCD_CS_CLK      RCC_APB2Periph_GPIOE

#define LCD_STD_PORT    GPIOE
#define LCD_STD_PIN     GPIO_Pin_8
#define LCD_STD_CLK     RCC_APB2Periph_GPIOE

#define LCD_SCLK_PORT   GPIOE
#define LCD_SCLK_PIN    GPIO_Pin_9
#define LCD_SCLK_CLK    RCC_APB2Periph_GPIOE

#define LCD_BLA_PORT    GPIOE
#define LCD_BLA_PIN     GPIO_Pin_10
#define LCD_BLA_CLK     RCC_APB2Periph_GPIOE

#define LCD_RST_PORT    GPIOE
#define LCD_RST_PIN     GPIO_Pin_11
#define LCD_RST_CLK     RCC_APB2Periph_GPIOE

/* ================================================================
 *  CH451 数码管+键盘 (实验7) — GPIOD GPIO模拟4线时序
 * ================================================================ */
#define CH451_DCLK_PORT GPIOD
#define CH451_DCLK_PIN  GPIO_Pin_8
#define CH451_DCLK_CLK  RCC_APB2Periph_GPIOD

#define CH451_DIN_PORT  GPIOD
#define CH451_DIN_PIN   GPIO_Pin_9
#define CH451_DIN_CLK   RCC_APB2Periph_GPIOD

#define CH451_DOUT_PORT GPIOD
#define CH451_DOUT_PIN  GPIO_Pin_10
#define CH451_DOUT_CLK  RCC_APB2Periph_GPIOD

#define CH451_LOAD_PORT GPIOD
#define CH451_LOAD_PIN  GPIO_Pin_11
#define CH451_LOAD_CLK  RCC_APB2Periph_GPIOD

/* ================================================================
 *  独立按键 IKEY0-3 (实验2,8) — GPIOF 下降沿EXTI 10K上拉
 * ================================================================ */
#define IKEY0_PORT      GPIOF
#define IKEY0_PIN       GPIO_Pin_0
#define IKEY0_CLK       RCC_APB2Periph_GPIOF
#define IKEY0_EXTI_LINE EXTI_Line0
#define IKEY0_EXTI_PORT GPIO_PortSourceGPIOF
#define IKEY0_EXTI_PIN  GPIO_PinSource0
#define IKEY0_IRQ       EXTI0_IRQn

#define IKEY1_PORT      GPIOF
#define IKEY1_PIN       GPIO_Pin_1
#define IKEY1_CLK       RCC_APB2Periph_GPIOF
#define IKEY1_EXTI_LINE EXTI_Line1
#define IKEY1_EXTI_PORT GPIO_PortSourceGPIOF
#define IKEY1_EXTI_PIN  GPIO_PinSource1
#define IKEY1_IRQ       EXTI1_IRQn

#define IKEY2_PORT      GPIOF
#define IKEY2_PIN       GPIO_Pin_2
#define IKEY2_CLK       RCC_APB2Periph_GPIOF
#define IKEY2_EXTI_LINE EXTI_Line2
#define IKEY2_EXTI_PORT GPIO_PortSourceGPIOF
#define IKEY2_EXTI_PIN  GPIO_PinSource2
#define IKEY2_IRQ       EXTI2_IRQn

#define IKEY3_PORT      GPIOF
#define IKEY3_PIN       GPIO_Pin_3
#define IKEY3_CLK       RCC_APB2Periph_GPIOF
#define IKEY3_EXTI_LINE EXTI_Line3
#define IKEY3_EXTI_PORT GPIO_PortSourceGPIOF
#define IKEY3_EXTI_PIN  GPIO_PinSource3
#define IKEY3_IRQ       EXTI3_IRQn

/* ================================================================
 *  拨码开关 SW0-7 (实验9) — GPIOF 10K下拉 ON=高
 * ================================================================ */
#define SW0_PORT        GPIOF
#define SW0_PIN         GPIO_Pin_4
#define SW0_CLK         RCC_APB2Periph_GPIOF

#define SW1_PORT        GPIOF
#define SW1_PIN         GPIO_Pin_5
#define SW1_CLK         RCC_APB2Periph_GPIOF

#define SW2_PORT        GPIOF
#define SW2_PIN         GPIO_Pin_6
#define SW2_CLK         RCC_APB2Periph_GPIOF

#define SW3_PORT        GPIOF
#define SW3_PIN         GPIO_Pin_7
#define SW3_CLK         RCC_APB2Periph_GPIOF

#define SW4_PORT        GPIOF
#define SW4_PIN         GPIO_Pin_8
#define SW4_CLK         RCC_APB2Periph_GPIOF

#define SW5_PORT        GPIOF
#define SW5_PIN         GPIO_Pin_9
#define SW5_CLK         RCC_APB2Periph_GPIOF

#define SW6_PORT        GPIOF
#define SW6_PIN         GPIO_Pin_10
#define SW6_CLK         RCC_APB2Periph_GPIOF

#define SW7_PORT        GPIOF
#define SW7_PIN         GPIO_Pin_11
#define SW7_CLK         RCC_APB2Periph_GPIOF

#define SW_ALL_PORT     GPIOF
#define SW_ALL_PINS     (GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                         GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11)

/* ================================================================
 *  继电器 (实验10) — GPIOG 高电平吸合 8050驱动
 * ================================================================ */
#define RELAY1_PORT     GPIOG
#define RELAY1_PIN      GPIO_Pin_2
#define RELAY1_CLK      RCC_APB2Periph_GPIOG

#define RELAY2_PORT     GPIOG
#define RELAY2_PIN      GPIO_Pin_3
#define RELAY2_CLK      RCC_APB2Periph_GPIOG

/* ================================================================
 *  DHT11 温湿度 (实验11) — GPIOF One-Wire 10K上拉
 * ================================================================ */
#define DHT11_PORT      GPIOF
#define DHT11_PIN       GPIO_Pin_12
#define DHT11_CLK       RCC_APB2Periph_GPIOF

/* ================================================================
 *  DS18B20 温度 (实验13) — GPIOF One-Wire 4.7K上拉
 * ================================================================ */
#define DS18B20_PORT    GPIOF
#define DS18B20_PIN     GPIO_Pin_13
#define DS18B20_CLK     RCC_APB2Periph_GPIOF

/* ================================================================
 *  TLC5615 DA (实验14) — SPI2
 * ================================================================ */
#define DA_CS_PORT      GPIOB
#define DA_CS_PIN       GPIO_Pin_12
#define DA_CS_CLK       RCC_APB2Periph_GPIOB

#define DA_CLK_PORT     GPIOB
#define DA_CLK_PIN      GPIO_Pin_13
#define DA_CLK_CLK      RCC_APB2Periph_GPIOB

#define DA_DATA_PORT    GPIOB
#define DA_DATA_PIN     GPIO_Pin_15
#define DA_DATA_CLK     RCC_APB2Periph_GPIOB

#define DA_SPI          SPI2
#define DA_SPI_CLK      RCC_APB1Periph_SPI2

/* ================================================================
 *  RS232 (实验15) — USART3
 * ================================================================ */
#define RS232_TX_PORT   GPIOB
#define RS232_TX_PIN    GPIO_Pin_10
#define RS232_TX_CLK    RCC_APB2Periph_GPIOB

#define RS232_RX_PORT   GPIOB
#define RS232_RX_PIN    GPIO_Pin_11
#define RS232_RX_CLK    RCC_APB2Periph_GPIOB

#define RS232_USART     USART3
#define RS232_USART_CLK RCC_APB1Periph_USART3

/* ================================================================
 *  RS485 (实验16) — USART4 + PG7方向控制
 * ================================================================ */
#define RS485_TX_PORT   GPIOC
#define RS485_TX_PIN    GPIO_Pin_10
#define RS485_TX_CLK    RCC_APB2Periph_GPIOC

#define RS485_RX_PORT   GPIOC
#define RS485_RX_PIN    GPIO_Pin_11
#define RS485_RX_CLK    RCC_APB2Periph_GPIOC

#define RS485_EN_PORT   GPIOG
#define RS485_EN_PIN    GPIO_Pin_7
#define RS485_EN_CLK    RCC_APB2Periph_GPIOG

#define RS485_USART     UART4
#define RS485_USART_CLK RCC_APB1Periph_UART4

/* ================================================================
 *  红外接收 (实验17) — GPIOD EXTI12 下降沿
 * ================================================================ */
#define IR_PORT         GPIOD
#define IR_PIN          GPIO_Pin_12
#define IR_CLK          RCC_APB2Periph_GPIOD
#define IR_EXTI_LINE    EXTI_Line12
#define IR_EXTI_PORT    GPIO_PortSourceGPIOD
#define IR_EXTI_PIN     GPIO_PinSource12
#define IR_IRQ          EXTI15_10_IRQn

/* ================================================================
 *  步进电机 (实验18) — GPIOE ULN2003
 * ================================================================ */
#define STEP_A_PORT     GPIOE
#define STEP_A_PIN      GPIO_Pin_12
#define STEP_A_CLK      RCC_APB2Periph_GPIOE

#define STEP_B_PORT     GPIOE
#define STEP_B_PIN      GPIO_Pin_13
#define STEP_B_CLK      RCC_APB2Periph_GPIOE

#define STEP_C_PORT     GPIOE
#define STEP_C_PIN      GPIO_Pin_14
#define STEP_C_CLK      RCC_APB2Periph_GPIOE

#define STEP_D_PORT     GPIOE
#define STEP_D_PIN      GPIO_Pin_15
#define STEP_D_CLK      RCC_APB2Periph_GPIOE

#define STEPPER_PORT    GPIOE
#define STEPPER_PINS    (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)

/* ================================================================
 *  编码器 (实验18) — PB6/PB7 TIM4_CH1/CH2 + PG8 Z相
 * ================================================================ */
#define ENC_A_PORT      GPIOB
#define ENC_A_PIN       GPIO_Pin_6
#define ENC_A_CLK       RCC_APB2Periph_GPIOB

#define ENC_B_PORT      GPIOB
#define ENC_B_PIN       GPIO_Pin_7
#define ENC_B_CLK       RCC_APB2Periph_GPIOB

#define ENC_Z_PORT      GPIOG
#define ENC_Z_PIN       GPIO_Pin_8
#define ENC_Z_CLK       RCC_APB2Periph_GPIOG

/* ================================================================
 *  直流电机 PWM (实验19) — PB0/PB1 TIM3_CH3/CH4
 * ================================================================ */
#define PWMA_PORT       GPIOB
#define PWMA_PIN        GPIO_Pin_0
#define PWMA_CLK        RCC_APB2Periph_GPIOB

#define PWMB_PORT       GPIOB
#define PWMB_PIN        GPIO_Pin_1
#define PWMB_CLK        RCC_APB2Periph_GPIOB

/* ================================================================
 *  舵机 (实验20) — PB9 TIM4_CH4 50Hz PWM
 * ================================================================ */
#define SERVO_PORT      GPIOB
#define SERVO_PIN       GPIO_Pin_9
#define SERVO_CLK       RCC_APB2Periph_GPIOB

/* ================================================================
 *  ADC (实验3,12) — PA1核心板光敏 + PC1主板热敏
 *  AD Switch = ON 启用核心板光敏
 * ================================================================ */
#define ADC_LIGHT_PORT  GPIOA
#define ADC_LIGHT_PIN   GPIO_Pin_1
#define ADC_LIGHT_CLK   RCC_APB2Periph_GPIOA
#define ADC_LIGHT_CH    ADC_Channel_1       /* ADC123_IN1 */

#define ADC_TEMP_PORT   GPIOC
#define ADC_TEMP_PIN    GPIO_Pin_1
#define ADC_TEMP_CLK    RCC_APB2Periph_GPIOC
#define ADC_TEMP_CH     ADC_Channel_11      /* ADC123_IN11 */

/* ================================================================
 *  调试串口 (实验4) — USART1 PA9/PA10 核心板CH340G
 * ================================================================ */
#define DBG_TX_PORT     GPIOA
#define DBG_TX_PIN      GPIO_Pin_9
#define DBG_TX_CLK      RCC_APB2Periph_GPIOA

#define DBG_RX_PORT     GPIOA
#define DBG_RX_PIN      GPIO_Pin_10
#define DBG_RX_CLK      RCC_APB2Periph_GPIOA

#define DBG_USART       USART1
#define DBG_USART_CLK   RCC_APB2Periph_USART1

/* ================================================================
 *  WKUP 模式切换按键 (实验2,21) — PA0 核心板板载
 * ================================================================ */
#define WKUP_PORT       GPIOA
#define WKUP_PIN        GPIO_Pin_0
#define WKUP_CLK        RCC_APB2Periph_GPIOA

/* ================================================================
 *  定时器
 * ================================================================ */
/* TIM3: 系统时基(0.5s Update IRQ) + DC电机PWM(CH3/CH4) */
#define TIM3_SYS        TIM3
#define TIM3_SYS_CLK    RCC_APB1Periph_TIM3
#define TIM3_PWMA_CH    TIM_Channel_3
#define TIM3_PWMB_CH    TIM_Channel_4

/* TIM4: 交通灯时基(Update IRQ 50Hz) + 舵机PWM(CH3) + 编码器捕获(CH1/CH2) */
#define TIM4_SYS        TIM4
#define TIM4_SYS_CLK    RCC_APB1Periph_TIM4
#define TIM4_SERVO_CH   TIM_Channel_3
#define TIM4_ENC_A_CH   TIM_Channel_1
#define TIM4_ENC_B_CH   TIM_Channel_2

#endif /* __PIN_CONFIG_H */
