#include "main.h"

static uint16_t leds[] = { LED_RED, LED_GREEN, LED_BLUE };
static int8_t   leds_counter;
static int8_t   leds_direction;

/* 1. Flash LEDs on extension board from 1 to 8 in an endless loop with 1 sec pause */
/* 2. Each time light only one LED */
/* 3. On user button press change LED flashing direction */
static
int configure_leds()
{
   GPIO_InitTypeDef leds_init_structure;
   /* Enable clocking for LEDS */
   RCC_AHB1PeriphClockCmd(LEDS_GPIO_CLK, ENABLE);
   leds_init_structure.GPIO_Pin = LED_RED | LED_GREEN | LED_BLUE;
   leds_init_structure.GPIO_Mode = GPIO_Mode_OUT;
   leds_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
   leds_init_structure.GPIO_OType = GPIO_OType_PP;
   GPIO_Init(LEDS_GPIO_PORT, &leds_init_structure);
   GPIO_SetBits(LEDS_GPIO_PORT, LED_RED | LED_GREEN | LED_BLUE);

   return 0;
}

static
int configure_buttons()
{
   GPIO_InitTypeDef buttons_init_structure;
   NVIC_InitTypeDef buttons_nvic_init_structure;
   EXTI_InitTypeDef buttons_exti_init_structure;

   /* Enable clocking for Buttons */
   RCC_AHB1PeriphClockCmd(BTNS_GPIO_CLOCK, ENABLE);
   /* Init pins */
   buttons_init_structure.GPIO_Pin   = BTN_LEFT | BTN_RIGHT;
   /* Set mode to input */
   buttons_init_structure.GPIO_Mode  = GPIO_Mode_IN;
   buttons_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
   buttons_init_structure.GPIO_OType = GPIO_OType_PP;
   buttons_init_structure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(BTNS_GPIO_PORT, &buttons_init_structure);

   /* Configure button interrupts (only left button)*/
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
   /* Connect EXTI Line0 to PE0 pin */
   SYSCFG_EXTILineConfig(BTNS_EXTI_PORT, BTN_LEFT_PIN_SOURCE);

   /* Configure EXTI Line0 */
   buttons_exti_init_structure.EXTI_Line    = BTN_LEFT_EXTI_LINE;
   buttons_exti_init_structure.EXTI_Mode    = EXTI_Mode_Interrupt;
   buttons_exti_init_structure.EXTI_Trigger = EXTI_Trigger_Rising;
   buttons_exti_init_structure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&buttons_exti_init_structure);

   /* Set interrupts to NVIC */
   buttons_nvic_init_structure.NVIC_IRQChannel = EXTI0_IRQn;
   buttons_nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0x01;
   buttons_nvic_init_structure.NVIC_IRQChannelSubPriority = 0x01;
   buttons_nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&buttons_nvic_init_structure);

   return 0;
}

static
int configure_timers()
{
   /* Timer  */
   TIM_TimeBaseInitTypeDef timer_init_structure;
   NVIC_InitTypeDef        timer_nvic_init_structure;

   /* Initialize peripheral clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   /* Initialize timer */
   timer_init_structure.TIM_Prescaler     = 84;  /* Scale value to microseconds */
   timer_init_structure.TIM_CounterMode   = TIM_CounterMode_Up;
   timer_init_structure.TIM_Period        = 1000000;   /* Gives us a second interval */
   timer_init_structure.TIM_ClockDivision = TIM_CKD_DIV1; /* Tell timer to divide clocks */
   timer_init_structure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(TIM2, &timer_init_structure);

   /* Configure timer interrupts */
   timer_nvic_init_structure.NVIC_IRQChannel = TIM2_IRQn;
   timer_nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0;
   timer_nvic_init_structure.NVIC_IRQChannelSubPriority = 1;
   timer_nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&timer_nvic_init_structure);

   /* Start timer */
   TIM_Cmd(TIM2, ENABLE);

   return 0;
}

void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(BTN_LEFT_EXTI_LINE) != RESET)
   {
      EXTI_ClearITPendingBit(BTN_LEFT_EXTI_LINE);

      leds_direction = -leds_direction;
   }
}

void TIM2_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

      GPIO_SetBits(LEDS_GPIO_PORT, leds[leds_counter]);
      leds_counter = (3 + (leds_counter - leds_direction) % 3) % 3;
      GPIO_ResetBits(LEDS_GPIO_PORT, leds[leds_counter]);
   }
}

int main(void)
{
   leds_counter = 0;
   leds_direction = 1;

   configure_leds();
   configure_buttons();
   configure_timers();

   for (;;)
   {
   }
}
