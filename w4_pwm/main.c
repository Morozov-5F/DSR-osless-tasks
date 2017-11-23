#include "main.h"

/* static uint16_t leds[] = { LED_RED, LED_GREEN, LED_BLUE }; */
/* static int8_t   leds_counter; */
/* static int8_t   leds_direction; */

/* PWM frequency */
#define PWM_FREQUENCY_HZ      40

static
int configure_leds()
{
   GPIO_InitTypeDef gpio_init;
   GPIO_StructInit(&gpio_init);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
   /* GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1); */
   /* GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); */

   /* Initialize GPIOs for an alternative function (TIM1 PWM output) */
   gpio_init.GPIO_Pin   = GPIO_Pin_8 /* | GPIO_Pin_9 | GPIO_Pin_10 */;
   gpio_init.GPIO_Mode  = GPIO_Mode_AF;
   gpio_init.GPIO_OType = GPIO_OType_PP;
   gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
   gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &gpio_init);
   return 0;
}

static
int configure_timers()
{
   /* Timer  */
   TIM_TimeBaseInitTypeDef tim_init;

   TIM_TimeBaseStructInit(&tim_init);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

   tim_init.TIM_Prescaler         = 1680; /* One microsecond resolution */
   tim_init.TIM_CounterMode       = TIM_CounterMode_Up;
   tim_init.TIM_Period            = (uint16_t)(100000 / PWM_FREQUENCY_HZ - 1);
   tim_init.TIM_ClockDivision     = TIM_CKD_DIV1;
   tim_init.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(TIM1, &tim_init);

   return 0;
}

static
int configure_pwm()
{
   TIM_OCInitTypeDef tim_oc_init;

   TIM_OCStructInit(&tim_oc_init);

   tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM1;
   tim_oc_init.TIM_Pulse       = (uint16_t)(100000 / PWM_FREQUENCY_HZ - 1) * 0.1;
   tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
   tim_oc_init.TIM_OCPolarity  = TIM_OCPolarity_Low;

   TIM_OC1Init(TIM1, &tim_oc_init);
   TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
   tim_oc_init.TIM_Pulse       = 0;
   TIM_OC2Init(TIM1, &tim_oc_init);
   TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
   tim_oc_init.TIM_Pulse       = 0;
   TIM_OC3Init(TIM1, &tim_oc_init);
   TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

   TIM_CtrlPWMOutputs(TIM1, ENABLE);
   TIM_Cmd(TIM1, ENABLE);

   return 0;
}

void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(BTN_LEFT_EXTI_LINE) != RESET)
   {
      EXTI_ClearITPendingBit(BTN_LEFT_EXTI_LINE);

   }
}

int main(void)
{
   configure_timers();
   configure_pwm();

   configure_leds();
   /* configure_buttons(); */

   for (;;);
}
