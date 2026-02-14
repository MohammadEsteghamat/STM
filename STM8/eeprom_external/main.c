#include "stm8s.h"
#include "tm1637.h"

int main( void )
{
    /* Infinite loop */
  tm1637_t disp;
  uint8_t data =  0;
  // CLK------------------------------Begin-----------------------------CLK //
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
  // CLK------------------------------End-------------------------------CLK //
  
  // tm1637---------------------------Begin--------------------------tm1637 //
  tm1637_init(&disp,GPIOC,GPIO_PIN_6,GPIOC,GPIO_PIN_7);
  uint8_t strt[] = {SegS,SegT,SegR,SegT};
  tm1637_write_segment(&disp,strt);
  tm1637_show_zero(&disp,1);
  // tm1637--------------------------End-----------------------------tm1637//

  // FLASH---------------------------Begin---------------------------FLASH //
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  // FLASH---------------------------End------------------------------FLASH//
  tm1637_write_int(&disp,data);
  delay_ms(1000);
  
  data = 10;
  tm1637_write_int(&disp,data);
  delay_ms(1000);
  
  data=FLASH_ReadByte(0x004000);
  tm1637_write_int(&disp,data);
  delay_ms(1000);
  
  FLASH_ProgramByte(0x004000,35);
  delay_ms(10);
  
  data=FLASH_ReadByte(0x004000);
  tm1637_write_int(&disp,data);
  delay_ms(1000);
  
  data = 100;
  tm1637_write_int(&disp,data);
  delay_ms(1000);
  while(1){
 
  }
  
  return 0;
}
