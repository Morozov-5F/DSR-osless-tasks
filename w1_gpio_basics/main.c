#include "main.h"

#define SWITCH_DELAY 500000

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* LEDs array to toggle between them */
  uint16_t leds[3] = { LED_RED, LED_GREEN, LED_BLUE };
  /* LED to toggle during iteration */
  uint8_t  current_led = 0;

  /* Enable peripheral clock for LEDs port */
  RCC_AHB1PeriphClockCmd(LEDS_CLK, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = LED_RED | LED_GREEN | LED_BLUE;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LEDS_PORT, &GPIO_InitStructure);

  /* Turn all the leds off */
  GPIO_SetBits(LEDS_PORT, LED_RED | LED_GREEN | LED_BLUE);

  while (1)
  {
    int i;

    /* Switch the LED on */
    GPIO_ResetBits(LEDS_PORT, leds[current_led % 3]);
    for (i = 0; i < SWITCH_DELAY; i++);

    /* Switch the LED off */
    GPIO_ResetBits(LEDS_PORT, leds[current_led++ % 3]);
    for (i = 0; i < SWITCH_DELAY; i++);
  }
}
