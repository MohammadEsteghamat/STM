#include "stm8s.h"

void clk_HSE();
void key_led();
void main(void)
{

  clk_HSE();
  key_led();
  while (1)
  {
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_6)==0)
    {
      GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    }
    if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_7)==0)
    {
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
 GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT); 
 GPIO_Init(GPIOC,GPIO_PIN_7,GPIO_MODE_IN_FL_NO_IT); 

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
