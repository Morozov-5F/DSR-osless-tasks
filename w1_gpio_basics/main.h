#ifndef __LEDS_AND_BUTTON_H
#define __LEDS_AND_BUTTON_H

#include "stm32f4xx.h"
#include "stm32f4_eval_board.h"
#if defined (USE_STM324xG_EVAL)
  #include "stm324xg_eval.h"

#elif defined (USE_STM32F4DISC_EVAL)
  #include "stm32f4disc_eval.h"

#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif

#include <math.h>

#endif
