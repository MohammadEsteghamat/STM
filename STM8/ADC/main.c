#include "stm8s.h"
#include "tm1637.h"

void clk_HSE();

tm1637_t disp;
int i;
float d;
char buffer[50];
void clk_HSE();
void adc_init();
void main(void)
{

  clk_HSE();
  tm1637_init(&disp,GPIOD,GPIO_PIN_4,GPIOC,GPIO_PIN_3);
  uint8_t strt[] = {SegS,SegT,SegR,SegT};
  tm1637_write_segment(&disp,strt);
  tm1637_fill(&disp,1);
  tm1637_show_zero(&disp,0);
  clk_HSE();
  adc_init();
  while (1)
  {
    
    ADC1_StartConversion();
    while(ADC1_GetFlagStatus(ADC1_FLAG_EOC)==0);
    i=ADC1_GetConversionValue();
    ADC1_ClearFlag(ADC1_FLAG_EOC);
    d=i*(3.3/1023)*100;
    tm1637_write_int(&disp,i);
    delay_ms(100);
    
  }
  
}
void clk_HSE()
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); //16 Mhz
}
void adc_init()
{
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
               ADC1_CHANNEL_2,
               ADC1_PRESSEL_FCPU_D18, 
               ADC1_EXTTRIG_GPIO, 
               DISABLE,ADC1_ALIGN_RIGHT, 
               ADC1_SCHMITTTRIG_CHANNEL0, 
               DISABLE);
 ADC1_Cmd(ENABLE);
 
  
}



