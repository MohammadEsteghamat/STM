#ifndef _TM1637_H_
#define _TM1637_H_
#include "stm8s.h"

#define  tm_segment  4

#define  Seg0  0b00111111
#define  Seg1  0b00000110
#define  Seg2  0b01011011
#define  Seg3  0b01001111
#define  Seg4  0b01100110
#define  Seg5  0b01101101
#define  Seg6  0b01111101
#define  Seg7  0b00000111
#define  Seg8  0b01111111
#define  Seg9  0b01101111
#define  SegA  0b01110111
#define  SegB  0b01111100
#define  SegC  0b00111001
#define  SegD  0b01011110
#define  SegE  0b01111001
#define  SegF  0b01110001
#define  SegG  0b00111101
#define  SegH  0b01110100
#define  SegI  0b00000110
#define  SegJ  0b00011111
#define  SegK  0b01110000
#define  SegL  0b00111000
#define  SegM  0b01010101
#define  SegN  0b01010100
#define  SegO  0b01011100
#define  SegP  0b01110011
#define  SegQ  0b01100111
#define  SegR  80
#define  SegS  109
#define  SegT  120
#define  SegU  0b00011100
#define  SegV  0b00111110
#define  SegW  0b01111110
#define  SegX  0b01110110
#define  SegY  0b01101110
#define  SegZ  0
#define  Seg_  0b00001000

//####################################################################################################################

typedef struct
{
  uint8_t               lock:1;
  uint8_t               brightness;
  char                  show_zero:1;    
  GPIO_TypeDef          *gpio_clk;
  GPIO_TypeDef          *gpio_dat;
  GPIO_Pin_TypeDef      pin_clk;   
  GPIO_Pin_TypeDef      pin_dat;  
  
}tm1637_t;

//####################################################################################################################
void delay_ms(unsigned long int n);
void tm1637_init(tm1637_t *tm1637, GPIO_TypeDef *gpio_clk, GPIO_Pin_TypeDef pin_clk, GPIO_TypeDef *gpio_dat, GPIO_Pin_TypeDef pin_dat);
void tm1637_brightness(tm1637_t *tm1637, uint8_t brightness_0_to_7);
void tm1637_write_segment(tm1637_t *tm1637, const uint8_t *segments);
void tm1637_fill(tm1637_t *tm1637, char enable);
void tm1637_write_int(tm1637_t *tm1637, int32_t digit);
void tm1637_show_zero(tm1637_t *tm1637, char enable);
//####################################################################################################################

#endif
