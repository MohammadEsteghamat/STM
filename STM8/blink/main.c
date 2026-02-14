#include "stm8s.h"
void delay();
void clk_HSE();
void clk_HSI();



void main(void)
{
  /* Infinite loop */
  GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);
  clk_HSE();
 // clk_HSI();
  while (1)
  {
    GPIO_WriteReverse(GPIOD,GPIO_PIN_2);
    
   delay(10000); 
  }
  
}
void delay(unsigned int n)
{

while (n-->0);
}
void clk_HSE(void)
{

CLK_HSECmd(ENABLE);
CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV16);



}
void clk_HSI(void)
{
  CLK_HSICmd(ENABLE);
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);
CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);

  
  
}