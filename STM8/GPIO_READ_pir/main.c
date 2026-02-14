#include "stm8s.h"
void delay(unsigned int n)
{

while (n-->0);
}
void clk_HSE();
void key_led();
void main(void)
{

  clk_HSE();
  key_led();
  GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
  delay(10000);
   
     GPIO_WriteLow(GPIOD,GPIO_PIN_4); 
    
  while (1)
  {
    if(GPIO_ReadInputPin(GPIOD, GPIO_PIN_3)==0)
    {
      GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    }
    else{
       GPIO_WriteLow(GPIOD,GPIO_PIN_4);
    }
   

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
  GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT); 
 GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT); 
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
