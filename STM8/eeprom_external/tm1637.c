
#include "tm1637.h"
#include "tm1637_config.h"

const uint8_t _tm1637_digit[] =
  {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
const uint8_t _tm1637_on[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t _tm1637_off[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const uint8_t fill_off[4] = {0x00, 0x00, 0x00, 0x00};
const uint8_t _tm1637_minus = 0x40;
const uint8_t _tm1637_dot = 0x80;
void delay_ms(unsigned long int n){
  unsigned long int x = 0;
  unsigned long fcpu = 16000000;
  x = fcpu / 1000;
  x = x / 50;
  n = n * x;
  while(n-- > 0){} 
}

#define tm1637_delay_ms(x)  delay_ms(x)


#define TM1637_COMM1    0x40
#define TM1637_COMM2    0xC0
#define TM1637_COMM3    0x80

 
//#######################################################################################################################
void tm1637_delay_us(uint32_t delay)
{
  while (delay > 0)
  {
    delay--;
    nop();nop();nop();nop();
  }
}
//#######################################################################################################################
void tm1637_start(tm1637_t *tm1637)
{
  GPIO_WriteLow(tm1637->gpio_dat,tm1637->pin_dat);
  tm1637_delay_us(_TM1637_BIT_DELAY);
}
//#######################################################################################################################
void tm1637_stop(tm1637_t *tm1637)
{
  GPIO_WriteLow(tm1637->gpio_dat,tm1637->pin_dat);
  tm1637_delay_us(_TM1637_BIT_DELAY);
  GPIO_WriteHigh(tm1637->gpio_clk,tm1637->pin_clk);
  tm1637_delay_us(_TM1637_BIT_DELAY);
  GPIO_WriteHigh(tm1637->gpio_dat,tm1637->pin_dat);
  tm1637_delay_us(_TM1637_BIT_DELAY);
}
//#######################################################################################################################
uint8_t tm1637_write_byte(tm1637_t *tm1637, uint8_t data)
{
  //  write 8 bit data
  for (uint8_t i = 0; i < 8; i++)
  {
    GPIO_WriteLow(tm1637->gpio_clk, tm1637->pin_clk);
    tm1637_delay_us(_TM1637_BIT_DELAY);
    if (data & 0x01)
      GPIO_WriteHigh(tm1637->gpio_dat, tm1637->pin_dat);
    else
      GPIO_WriteLow(tm1637->gpio_dat, tm1637->pin_dat);
    tm1637_delay_us(_TM1637_BIT_DELAY);
    GPIO_WriteHigh(tm1637->gpio_clk, tm1637->pin_clk);
    tm1637_delay_us(_TM1637_BIT_DELAY);
    data = data >> 1;
  }
  // wait for acknowledge
  GPIO_WriteLow(tm1637->gpio_clk, tm1637->pin_clk);
  GPIO_WriteHigh(tm1637->gpio_dat, tm1637->pin_dat);
  tm1637_delay_us(_TM1637_BIT_DELAY);
  GPIO_WriteHigh(tm1637->gpio_clk, tm1637->pin_clk);
  tm1637_delay_us(_TM1637_BIT_DELAY);
  uint8_t ack = GPIO_ReadInputPin(tm1637->gpio_dat, tm1637->pin_dat);
  if (ack == 0)
    GPIO_WriteLow(tm1637->gpio_dat, tm1637->pin_dat);
  tm1637_delay_us(_TM1637_BIT_DELAY);
  GPIO_WriteLow(tm1637->gpio_clk, tm1637->pin_clk);
  tm1637_delay_us(_TM1637_BIT_DELAY);
  return ack;
}
//#######################################################################################################################
void tm1637_lock(tm1637_t *tm1637)
{
  while (tm1637->lock == 1)
    tm1637_delay_ms(1);
  tm1637->lock = 1;  
}
//#######################################################################################################################
void tm1637_unlock(tm1637_t *tm1637)
{
  tm1637->lock = 0;  
}
//#######################################################################################################################
void tm1637_init(tm1637_t *tm1637, GPIO_TypeDef *gpio_clk, GPIO_Pin_TypeDef pin_clk, GPIO_TypeDef *gpio_dat, GPIO_Pin_TypeDef pin_dat)
{
  //  set max brightess
  tm1637_brightness(tm1637, 7);  
  tm1637_lock(tm1637);
  //  init gpio
  tm1637->gpio_clk = gpio_clk;
  tm1637->pin_clk = pin_clk;
  tm1637->gpio_dat = gpio_dat;
  tm1637->pin_dat = pin_dat;
  GPIO_Init(tm1637->gpio_clk,tm1637->pin_clk,GPIO_MODE_OUT_OD_LOW_FAST);
  GPIO_Init(tm1637->gpio_dat,tm1637->pin_dat,GPIO_MODE_OUT_OD_LOW_FAST);   
  tm1637_unlock(tm1637);
}
//#######################################################################################################################
void tm1637_brightness(tm1637_t *tm1637, uint8_t brightness_0_to_7)
{
  tm1637_lock(tm1637);
  tm1637->brightness = (brightness_0_to_7 & 0x7) | 0x08;
  tm1637_unlock(tm1637);
}
//#######################################################################################################################
void tm1637_write_raw(tm1637_t *tm1637, const uint8_t *raw)
{
 

  // write COMM1
  tm1637_start(tm1637);
  tm1637_write_byte(tm1637, TM1637_COMM1);
  tm1637_stop(tm1637);
  // write COMM2 + first digit address
  tm1637_start(tm1637);
  tm1637_write_byte(tm1637, TM1637_COMM2);
  // write the data bytes
  for (uint8_t k=0; k < tm_segment; ++k)
    tm1637_write_byte(tm1637, raw[k]);
  tm1637_stop(tm1637);
  // write COMM3 + brightness
  tm1637_start(tm1637);
  tm1637_write_byte(tm1637, TM1637_COMM3 + tm1637->brightness);
  tm1637_stop(tm1637);
}
//#######################################################################################################################
void tm1637_write_segment(tm1637_t *tm1637, const uint8_t *segments)
{
  tm1637_lock(tm1637);
  tm1637_write_raw(tm1637, segments);
  tm1637_unlock(tm1637);  
}
//#######################################################################################################################
void tm1637_fill(tm1637_t *tm1637, char enable)
{
	if (enable)
		tm1637_write_segment(tm1637, _tm1637_on);
	else
		tm1637_write_segment(tm1637, _tm1637_off);		
}
//#######################################################################################################################
void tm1637_write_int(tm1637_t *tm1637, int32_t digit)
{
  tm1637_lock(tm1637);
  uint8_t buffer[tm_segment] = {0};
  for(uint8_t i = tm_segment - 1;i<20;--i){
    buffer[i] = _tm1637_digit[digit % 10];
    digit /= 10;
  }
  if(tm1637->show_zero == 0){
    for(uint8_t i = 0 ;i<tm_segment;++i){
      if(buffer[i] == _tm1637_digit[0]){
        buffer[i] = SegZ;
      }
      else{
        break;
      }
    }
  }
  tm1637_write_raw(tm1637, buffer);              
  tm1637_unlock(tm1637);  
}
//#######################################################################################################################
void tm1637_show_zero(tm1637_t *tm1637, char enable)
{
  tm1637->show_zero = enable;
}
//#######################################################################################################################








