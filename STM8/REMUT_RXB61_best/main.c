
#include <stm8s.h>
#include "remut.h"

char he[3];
void delayq(unsigned int n)
{
  while (n-->0);
}

void bin_to_256(char * a,char * b){
  a[0] = (b[0] << 7)|(b[1] << 6)|(b[2] << 5)|(b[3] << 4)|(b[4] << 3)|(b[5] << 2)|(b[7] << 1)|(b[7] << 0); 
  a[1] = (b[8] << 7)|(b[9] << 6)|(b[10] << 5)|(b[11] << 4)|(b[12] << 3)|(b[13] << 2)|(b[14] << 1)|(b[15] << 0);
  a[2] = (b[16] << 7)|(b[17] << 6)|(b[18] << 5)|(b[19] << 4)|(0 << 3)|(0 << 2)|(0 << 1)|(0 << 0);
}
void chap_int(char a){
          UART1_SendData8(a/100+48);
          delayq(500);
          UART1_SendData8(a%100/10+48);
          delayq(500);
          UART1_SendData8(a%10+48);
          delayq(500);
          UART1_SendData8('-');
          delayq(500);
}

void uart_init()
{
   UART1_Init(9600,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,
              UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE,
              UART1_MODE_TXRX_ENABLE
              ); 
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE); 
  UART1_Cmd(ENABLE);
  enableInterrupts();
  
}

int main( void )
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
    GPIO_DeInit(GPIOD);
    GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOA,GPIO_PIN_1,GPIO_MODE_IN_FL_NO_IT);
 
    TIM1_DeInit(); 

 
    TIM1_TimeBaseInit(39, TIM1_COUNTERMODE_UP, 3, 0); 


    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); 

   
    enableInterrupts();

   
    TIM1_Cmd(ENABLE); 
    uart_init();
   
    GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    delayq(10);
    GPIO_WriteLow(GPIOD,GPIO_PIN_4);
   
     while(1){
       if(remut.read_remut==1){
       
        GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    
        GPIO_WriteLow(GPIOD,GPIO_PIN_4);
        bin_to_256(he,remut.data);
        chap_int(he[0]);
        chap_int(he[1]);
        chap_int(he[2]);
        UART1_SendData8('\n');
           remut.read_remut =0;
       }
    }
    return 0;
}


