#include "stm8s.h"
#include "tm1637.h"

void clk_HSE();

tm1637_t disp;
void main(void)
{

  clk_HSE();
  tm1637_init(&disp,GPIOD,GPIO_PIN_4,GPIOC,GPIO_PIN_3);
  uint8_t strt[] = {SegS,SegT,SegR,SegT};
  tm1637_write_segment(&disp,strt);
  tm1637_fill(&disp,1);
  tm1637_show_zero(&disp,0);

  while (1)
  {
    for(long int i = 0;i<10000;i++){
      tm1637_write_int(&disp,i);
      delay_ms(1000);
    }
  }
  
}
void clk_HSE()
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1); //16 Mhz
}


