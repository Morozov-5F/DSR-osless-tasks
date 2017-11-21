#include "main.h"

#define SWITCH_DELAY 500000

int main(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable peripheral clock for LEDs port */
  RCC_AHB1PeriphClockCmd(LEDS_CLK, ENABLE);

  /* Init leds */
  GPIO_InitStructure.GPIO_Pin   = LED_RED | LED_GREEN | LED_BLUE;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LEDS_PORT, &GPIO_InitStructure);

  while (1)
  {
	  int i;

      /* Switch LED on */
      GPIO_SetBits(LEDS_PORT, LED_RED | LED_GREEN | LED_BLUE);
	  for (i=0; i < SWITCH_DELAY; i++)
	  {
	    /* empty cycle */
	  }

	  /* Switch LED off */
	  GPIO_ResetBits(LEDS_PORT, LED_RED | LED_GREEN | LED_BLUE);
	  for (i=0; i < SWITCH_DELAY; i++)
	  {
	    /* empty cycle */
	  }
  }
}
