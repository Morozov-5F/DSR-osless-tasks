#ifndef __STM32_EVAL_BOARD_H__
#define __STM32_EVAL_BOARD_H__

/* Define LED Pins and ports */
#define LED_RED             GPIO_Pin_8
#define LED_GREEN           GPIO_Pin_9
#define LED_BLUE            GPIO_Pin_10

#define LEDS_PORT           GPIOA
#define LEDS_CLK            RCC_AHB1Periph_GPIOA

/* Timer defines */
#define EXTENSION_TIMER     TIM1
#define TIMER_CLK           RCC_APB1Periph_TIM1
#define TIMER_IRQn          TIM1_IRQn

/* Button defines */
#define BUTTON_LEFT         GPIO_Pin_0
#define BUTTON_RIGHT        GPIO_Pin_1

#define BTNS_PORT           GPIOE

#endif /* __STM32_EVAL_BOARD_H__ */