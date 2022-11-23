#ifndef SRC_IODEFINE_H_
#define SRC_IODEFINE_H_
#endif /* SRC_IODEFINE_H_ */

// LED
#define LED_RCC				RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
#define LED1_PORT			GPIOC
#define LED1_PIN			GPIO_Pin_13
#define LED2_PORT			GPIOC
#define LED2_PIN			GPIO_Pin_14
#define LED3_PORT			GPIOA
#define LED3_PIN			GPIO_Pin_0
#define LED4_PORT			GPIOA
#define LED4_PIN			GPIO_Pin_1

// SW
#define SW_RCC				RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
#define SW1_PORT			GPIOB
#define SW1_PIN				GPIO_Pin_5
#define SW2_PORT			GPIOC
#define SW2_PIN				GPIO_Pin_11
#define SW3_PORT			GPIOC
#define SW3_PIN				GPIO_Pin_10
#define SW1 				GPIO_ReadInputDataBit( SW1_PORT, SW1_PIN )
#define SW2					GPIO_ReadInputDataBit( SW2_PORT, SW2_PIN )
#define SW3					GPIO_ReadInputDataBit( SW3_PORT, SW3_PIN )
#define SW_ON				1
#define SW_OFF				0

// BUZZER
#define BUZZER_RCC			RCC_APB2Periph_GPIOA
#define BUZZER_PORT			GPIOA
#define BUZZER_PIN			GPIO_Pin_3
#define BUZZER_C4			261					// ド
#define BUZZER_D4			293					// レ
#define BUZZER_E4			329					// ミ
#define BUZZER_F4			349					// ファ
#define BUZZER_G4			391					// ソ
#define BUZZER_A4			440					// ラ
#define BUZZER_B4			493					// シ
#define BUZZER_C5			523					// ド

// BATTERY
#define BATTERY_RCC			RCC_APB2Periph_GPIOA
#define BATTERY_PORT		GPIOA
#define BATTERY_PIN			GPIO_Pin_4
#define BATTERY_ADC_CH		4

// MOTER_DRIVER
#define MOTER_RCC			RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
#define MOTER_EN_PORT		GPIOC
#define MOTER_EN_PIN		GPIO_Pin_6
#define MOTER_CW_L_PORT		GPIOC
#define MOTER_CW_L_PIN		GPIO_Pin_7
#define MOTER_CW_R_PORT		GPIOC
#define MOTER_CW_R_PIN		GPIO_Pin_8
#define MOTER_PWM_L_PORT	GPIOB
#define MOTER_PWM_L_PIN		GPIO_Pin_10
#define MOTER_PWM_R_PORT	GPIOC
#define MOTER_PWM_R_PIN		GPIO_Pin_9

// WALL_LED
#define WALL_LED_RCC		RCC_APB2Periph_GPIOB
#define WALL_LED1_PORT		GPIOB
#define WALL_LED1_PIN		GPIO_Pin_6
#define WALL_LED2_PORT		GPIOB
#define WALL_LED2_PIN		GPIO_Pin_7
#define WALL_LED3_PORT		GPIOB
#define WALL_LED3_PIN		GPIO_Pin_8
#define WALL_LED4_PORT		GPIOB
#define WALL_LED4_PIN		GPIO_Pin_9

// WALL_PT
#define WALL_PT_RCC			RCC_APB2Periph_GPIOC
#define WALL_PT1_PORT		GPIOC
#define WALL_PT1_PIN		GPIO_Pin_0
#define WALL_PT2_PORT		GPIOC
#define WALL_PT2_PIN		GPIO_Pin_1
#define WALL_PT3_PORT		GPIOC
#define WALL_PT3_PIN		GPIO_Pin_2
#define WALL_PT4_PORT		GPIOC
#define WALL_PT4_PIN		GPIO_Pin_3
#define WALL_PT1_ADC_CH		10					// 右前壁
#define WALL_PT2_ADC_CH		11					// 右横壁
#define WALL_PT3_ADC_CH		12					// 左横壁
#define WALL_PT4_ADC_CH		13					// 左前壁

// LCD
#define LCD_RCC				RCC_APB2Periph_GPIOC

