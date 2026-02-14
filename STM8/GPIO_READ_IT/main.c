#include "stm8s.h"

void clk_HSE();
void key_led();
void EXT_init();

void main(void)
{

  enableInterrupts();
  clk_HSE();
  EXT_init();
  key_led();
  while (1)
  {
 

  }
  
}
void clk_HSE()
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV16);
  
}
void key_led()
{
 GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST); 
 GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_IN_PU_IT); 
 GPIO_Init(GPIOC,GPIO_PIN_7,GPIO_MODE_IN_PU_IT); 

}
void EXT_init()
{
  
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);
   // EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);

}

