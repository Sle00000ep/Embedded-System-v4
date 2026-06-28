# V4核心板 UniBoard CORE — STM32F103ZET6 引脚与试验箱PORT映射表

> **数据来源**：V4版核心板原理图 "IO PORT" 部分（V4版核心板原理图-第二种_核心板UniBoard CORE STM32F103ZET6.PDF）
> **映射规律**：原理图标注 P0~P147 直接对应试验箱 PORT0~PORT147
> **日期**：2026-06-10

---

## 一、核心板概况

| 项目 | 详情 |
|------|------|
| MCU型号 | STM32F103ZET6 |
| 封装 | LQFP144 |
| 内核 | Cortex-M3, 72MHz |
| Flash/RAM | 512KB / 64KB |
| 核心板型号 | UniBoard CORE |
| IO引出方式 | 2组 2x37P 排针（共148个 PORT：P0~P147） |
| 信号引脚 | 118个 |
| 电源引脚 | 30个（GNDx14, 3V3x12, 5Vx2, VREFx2） |

### 核心板开关配置

| 开关 | 功能 | 本项目设置 |
|------|------|-----------|
| SW0 (Power) | 电源开关 | ON |
| SW1 (USB/UART) | USB转串口切换 | UART侧 |
| SW2 (BOOT0) | 启动模式选择 | OFF (Flash启动) |
| SW3 (LED Power) | PF0~PF15 LED电源 | **OFF**（释放PF口为GPIO） |
| AD Switch | 板载ADC电位器 | — |

---

## 二、完整 PORT → STM32 映射表

### CON1 侧 — PORT 0~73

| PORT | STM32端口 | 完整复用功能 | 类型 |
|------|-----------|-------------|:--:|
| P0 | **GND** | 电源地 | ⚡ |
| P1 | **GND** | 电源地 | ⚡ |
| P2 | **PB13** | SPI2_SCK / I2S2_CK / USART3_CTS / TIM1_CH1N | I/O |
| P3 | **PB12** | SPI2_NSS / I2S2_WS / I2C2_SMBAI / USART3_CK / TIM1_BKIN | I/O |
| P4 | **PB15** | SPI2_MOSI / I2S2_SD / TIM1_CH3N | I/O |
| P5 | **PB14** | SPI2_MISO / USART3_RTS / TIM1_CH2N | I/O |
| P6 | **PD9** | FSMC_D14 | I/O |
| P7 | **PD8** | FSMC_D13 | I/O |
| P8 | **PD11** | FSMC_A16 | I/O |
| P9 | **PD10** | FSMC_D15 | I/O |
| P10 | **PD13** | FSMC_A18 | I/O |
| P11 | **PD12** | FSMC_A17 | I/O |
| P12 | **3V3** | 电源 3.3V | ⚡ |
| P13 | **GND** | 电源地 | ⚡ |
| P14 | **PD15** | FSMC_D1 | I/O |
| P15 | **PD14** | FSMC_D0 | I/O |
| P16 | **PG3** | FSMC_A13 | I/O |
| P17 | **PG2** | FSMC_A12 | I/O |
| P18 | **PG5** | FSMC_A15 | I/O |
| P19 | **PG4** | FSMC_A14 | I/O |
| P20 | **PG7** | FSMC_INT3 | I/O |
| P21 | **PG6** | FSMC_INT2 | I/O |
| P22 | **GND** | 电源地 | ⚡ |
| P23 | **PG8** | —（仅GPIO） | I/O |
| P24 | **PC6** | I2S2_MCK / TIM8_CH1 / SDIO_D6 | I/O |
| P25 | **3V3** | 电源 3.3V | ⚡ |
| P26 | **PC8** | TIM8_CH3 / SDIO_D0 | I/O |
| P27 | **PC7** | I2S3_MCK / TIM8_CH2 / SDIO_D7 | I/O |
| P28 | **PA8** | USART1_CK / TIM1_CH1 / MCO | I/O |
| P29 | **PC9** | TIM8_CH4 / SDIO_D1 | I/O |
| P30 | **PA10** | USART1_RX / TIM1_CH3 | I/O |
| P31 | **PA9** | USART1_TX / TIM1_TX / TIM1_CH2 | I/O |
| P32 | **PA12** | USART1_RTS / CANTX / TIM1_ETR / USBDP | I/O |
| P33 | **PA11** | USART1_CTS / CANRX / TIM1_CH4 / USBDM | I/O |
| P34 | **NC** | 芯片空脚（内部未连接） | — |
| P35 | **PA13** | JTMS-SWDIO（🔒 调试器，禁止复用） | 🔒 |
| P36 | **3V3** | 电源 3.3V | ⚡ |
| P37 | **GND** | 电源地 | ⚡ |
| P38 | **PA14** | JTMS-SWCLK（🔒 调试器，禁止复用） | 🔒 |
| P39 | **PA15** | JTDI / SPI3_NSS / I2S3_WS | I/O |
| P40 | **PC10** | USART4_TX / SDIO_D2 | I/O |
| P41 | **PC11** | USART4_RX / SDIO_D3 | I/O |
| P42 | **PC12** | USART5_TX / SDIO_CK | I/O |
| P43 | **PD0** | FSMC_D2 | I/O |
| P44 | **PD1** | FSMC_D3 | I/O |
| P45 | **PD2** | TIM3_ETR / USART5_RX / SDIO_CMD | I/O |
| P46 | **PD3** | FSMC_CLK | I/O |
| P47 | **PD4** | FSMC_NOE | I/O |
| P48 | **PD5** | FSMC_NWE | I/O |
| P49 | **GND** | 电源地 | ⚡ |
| P50 | **3V3** | 电源 3.3V | ⚡ |
| P51 | **PD6** | FSMC_NWAIT | I/O |
| P52 | **PD7** | FSMC_NE1 / FSMC_NCE2 | I/O |
| P53 | **PG9** | FSMC_NE2 / FSMC_NCE3 | I/O |
| P54 | **PG10** | FSMC_NCE4_1 / FSMC_NE3 | I/O |
| P55 | **PG11** | FSMC_NCE4_2 | I/O |
| P56 | **PG12** | FSMC_NE4 | I/O |
| P57 | **PG13** | FSMC_A24 | I/O |
| P58 | **PG14** | FSMC_A25 | I/O |
| P59 | **GND** | 电源地 | ⚡ |
| P60 | **3V3** | 电源 3.3V | ⚡ |
| P61 | **PG15** | —（仅GPIO） | I/O |
| P62 | **PB3** | JTDO / TRACESWO / SPI3_SCK / I2S3_CK | I/O |
| P63 | **PB4** | JNTRST / SPI3_MISO | I/O |
| P64 | **PB5** | I2C1_SMBAI / SPI3_MOSI / I2S3_SD | I/O |
| P65 | **PB6** | I2C1_SCL / TIM4_CH1 | I/O |
| P66 | **PB7** | I2C1_SDA / FSMC_NADV / TIM4_CH2 | I/O |
| P67 | **BOOT0** | 启动模式选择（🔒 接地=Flash启动） | 🔒 |
| P68 | **PB8** | TIM4_CH3 / SDIO_D4 | I/O |
| P69 | **PB9** | TIM4_CH4 / SDIO_D5 | I/O |
| P70 | **PE0** | TIM4_ETR / FSMC_NBL0 | I/O |
| P71 | **PE1** | FSMC_NBL1 | I/O |
| P72 | **GND** | 电源地 | ⚡ |
| P73 | **3V3** | 电源 3.3V | ⚡ |

### CON2 侧 — PORT 74~147

| PORT | STM32端口 | 完整复用功能 | 类型 |
|------|-----------|-------------|:--:|
| P74 | **5V** | 5V 电源输出 | ⚡ |
| P75 | **5V** | 5V 电源输出 | ⚡ |
| P76 | **3V3** | 电源 3.3V | ⚡ |
| P77 | **GND** | 电源地 | ⚡ |
| P78 | **PB11** | I2C2_SDA / USART3_RX | I/O |
| P79 | **PB10** | I2C2_SCL / USART3_TX | I/O |
| P80 | **PE15** | FSMC_D12 | I/O |
| P81 | **PE14** | FSMC_D11 | I/O |
| P82 | **PE13** | FSMC_D10 | I/O |
| P83 | **PE12** | FSMC_D9 | I/O |
| P84 | **PE11** | FSMC_D8 | I/O |
| P85 | **PE10** | FSMC_D7 | I/O |
| P86 | **3V3** | 电源 3.3V | ⚡ |
| P87 | **GND** | 电源地 | ⚡ |
| P88 | **PE9** | FSMC_D6 | I/O |
| P89 | **PE8** | FSMC_D5 | I/O |
| P90 | **PE7** | FSMC_D4 | I/O |
| P91 | **PG1** | FSMC_A11 | I/O |
| P92 | **PG0** | FSMC_A10 | I/O |
| P93 | **PF15** | FSMC_A9 / LED15 | I/O |
| P94 | **PF14** | FSMC_A8 / LED14 | I/O |
| P95 | **PF13** | FSMC_A7 / LED13 | I/O |
| P96 | **3V3** | 电源 3.3V | ⚡ |
| P97 | **GND** | 电源地 | ⚡ |
| P98 | **PF12** | FSMC_A6 / LED12 | I/O |
| P99 | **PF11** | FSMC_NIOS16 / LED11 | I/O |
| P100 | **PB2** | BOOT1（🔒 启动配置，接地） | 🔒 |
| P101 | **PB1** | ADC12_IN9 / TIM3_CH4 / TIM8_CH3N | I/O |
| P102 | **PB0** | ADC12_IN8 / TIM3_CH3 / TIM8_CH2N | I/O |
| P103 | **PC5** | ADC12_IN15 | I/O |
| P104 | **PC4** | ADC12_IN14 | I/O |
| P105 | **PA7** | SPI1_MOSI / TIM8_CH1N / ADC12_IN7 / TIM3_CH2 | I/O |
| P106 | **PA6** | SPI1_MISO / TIM8_BKIN / ADC12_IN6 / TIM3_CH1 | I/O |
| P107 | **PA5** | SPI1_SCK / DAC_OUT2 / ADC12_IN5 | I/O |
| P108 | **PA4** | SPI1_NSS / DAC_OUT1 / USART2_CK / ADC12_IN4 | I/O |
| P109 | **3V3** | 电源 3.3V | ⚡ |
| P110 | **GND** | 电源地 | ⚡ |
| P111 | **PA3** | USART2_RX / TIM5_CH4 / ADC123_IN3 / TIM2_CH4 | I/O |
| P112 | **PA1** | USART2_RTS / ADC123_IN1 / TIM5_CH2 / TIM2_CH2 | I/O |
| P113 | **PA2** | USART2_TX / TIM5_CH3 / ADC123_IN2 / TIM2_CH3 | I/O |
| P114 | **3V3** | 电源 3.3V | ⚡ |
| P115 | **PA0** | WKUP / USART2_CTS / ADC123_IN0 / TIM5_CH1 / TIM2_CH1_ETR / TIM8_ETR | I/O |
| P116 | **VREF-** | ADC负参考电压（🔒 勿用作GPIO） | 🔒 |
| P117 | **VREF+** | ADC正参考电压（🔒 勿用作GPIO） | 🔒 |
| P118 | **PC3** | ADC123_IN13 | I/O |
| P119 | **GND** | 电源地 | ⚡ |
| P120 | **PC1** | ADC123_IN11 | I/O |
| P121 | **PC2** | ADC123_IN12 | I/O |
| P122 | **NRST** | 系统复位（🔒 勿用作GPIO） | 🔒 |
| P123 | **PC0** | ADC123_IN10 | I/O |
| P124 | **OSCIN** | 8MHz 主晶振输入（🔒 勿用作GPIO） | 🔒 |
| P125 | **OSCOUT** | 8MHz 主晶振输出（🔒 勿用作GPIO） | 🔒 |
| P126 | **PF9** | ADC3_IN7 / FSMC_CD / LED9 | I/O |
| P127 | **PF10** | ADC3_IN8 / FSMC_INTR / LED10 | I/O |
| P128 | **PF7** | ADC3_IN5 / FSMC_NREG / LED7 | I/O |
| P129 | **PF8** | ADC3_IN6 / FSMC_NIOWR / LED8 | I/O |
| P130 | **3V3** | 电源 3.3V | ⚡ |
| P131 | **PF6** | ADC3_IN4 / FSMC_NIORD / LED6 | I/O |
| P132 | **PF5** | FSMC_A5 / LED5 | I/O |
| P133 | **GND** | 电源地 | ⚡ |
| P134 | **PF3** | FSMC_A3 / LED3 | I/O |
| P135 | **PF4** | FSMC_A4 / LED4 | I/O |
| P136 | **PF1** | FSMC_A1 / LED1 | I/O |
| P137 | **PF2** | FSMC_A2 / LED2 | I/O |
| P138 | **PC15** | OSC32_OUT（🔒 RTC晶振，禁止复用） | 🔒 |
| P139 | **PF0** | FSMC_A0 / LED0 | I/O |
| P140 | **PC13** | TAMPER-RTC | I/O |
| P141 | **PC14** | OSC32_IN（🔒 RTC晶振，禁止复用） | 🔒 |
| P142 | **PE6** | TRACED3 / FSMC_A22 | I/O |
| P143 | **VBAT** | RTC后备电池（🔒 电池供电，勿用作GPIO） | 🔒 |
| P144 | **PE4** | TRACED1 / FSMC_A20 | I/O |
| P145 | **PE5** | TRACED2 / FSMC_A21 | I/O |
| P146 | **PE2** | TRACECK / FSMC_A23 | I/O |
| P147 | **PE3** | TRACED0 / FSMC_A19 | I/O |

---

## 三、已纠正的11个端口

| PORT | 原来 | 纠正为 | 来源 |
|------|------|--------|------|
| P12 | ⚠️ 待核查 | **3V3** | 用户对照原理图确认 |
| P25 | ⚠️ 待核查 | **3V3** | 用户对照原理图确认 |
| P36 | ⚠️ 待核查 | **3V3** | 用户对照原理图确认 |
| P67 | ⚠️ 待核查 | **BOOT0** | 用户对照原理图确认 |
| P100 | ⚠️ 待核查 | **PB2** (BOOT1) | 用户对照原理图确认 |
| P112 | ⚠️ 待核查 | **PA1** | 用户对照原理图确认 |
| P114 | ⚠️ 待核查 | **3V3** | 用户对照原理图确认 |
| P116 | ⚠️ 待核查 | **VREF-** | 用户对照原理图确认 |
| P122 | ⚠️ 待核查 | **NRST** | 用户对照原理图确认 |
| P124 | ⚠️ 待核查 | **OSCIN** | 用户对照原理图确认 |
| P125 | ⚠️ 待核查 | **OSCOUT** | 用户对照原理图确认 |
| P130 | GND | **3V3** | 用户对照原理图纠正 |

---

## 四、禁止复用的引脚汇总

| PORT | 引脚 | 原因 |
|------|------|------|
| P34 | NC | 芯片空脚，内部未连接 |
| P35 | PA13 | SWDIO — 调试器数据线 |
| P38 | PA14 | SWCLK — 调试器时钟线 |
| P67 | BOOT0 | 启动模式选择（须接地） |
| P100 | PB2/BOOT1 | 启动配置（须接地） |
| P116 | VREF- | ADC负参考电压 |
| P117 | VREF+ | ADC正参考电压 |
| P122 | NRST | 系统复位引脚 |
| P124 | OSCIN | 8MHz主晶振 |
| P125 | OSCOUT | 8MHz主晶振 |
| P138 | PC15 | RTC 32.768KHz晶振 |
| P141 | PC14 | RTC 32.768KHz晶振 |
| P143 | VBAT | RTC后备电池 |

---

## 五、统计

| 类别 | 数量 |
|------|:----:|
| 总PORT数 | 148 (P0~P147) |
| 可用 GPIO / 功能引脚 | 118 |
| 电源引脚（GND/3V3/5V/VREF） | 30 |
| 禁止复用引脚 | 14（含 NC） |
| 确认率 | **100%** |
