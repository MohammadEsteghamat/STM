#include "stm8s.h"
const unsigned char num[]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void clk_HSE();
void Tim2_init();

void gpio_init();
void main(void)
{
  /* Infinite loop */
  clk_HSE();
  Tim2_init();
 
  gpio_init();
  
  while (1)
  {
    
  }
  
}
void clk_HSE()
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV2);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2,ENABLE);

}
void Tim2_init()
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_256,200);
  TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
  TIM2_Cmd(ENABLE);
  enableInterrupts();
  
}

void gpio_init()
{
  GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
}