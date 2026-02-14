#include "stm8s.h"
void clk_HSE();
void delay();
void led();
char i=0;
void main(void)
{
  /* Infinite loop */
  clk_HSE();
  led();
  while (1)
  {
    for(i=0;i<255;i++)
    {
     GPIO_Write(GPIOD,i); 
     delay(10000);
      
    }
  }
  
}
void clk_HSE(void)
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV16);
  
  
}
void delay(unsigned int n)
{
  while (n-->0);
  
}
void led(void)
{
  GPIO_Init(GPIOD,GPIO_PIN_ALL,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Write(GPIOD,i);

  
}
