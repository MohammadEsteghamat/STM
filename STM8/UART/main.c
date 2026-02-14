#include "stm8s.h"
#include "stdio.h"

char buffer[20];
float d;
uint16_t l;
char i=0;

void clk_HSE();
void delay();

void uart_init();
void main(void)
{
  /* Infinite loop */
  clk_HSE();
  uart_init();

  GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
  while (1)
  {

  
   sprintf(buffer,"TEMP : %.2f",123.22);
   while(buffer[i]!=0)
   {
     UART1_SendData8(buffer[i]);
     i++;
     delay(500);
     
   }
   UART1_SendData8('\n');
   i=0;
    
  }
  
}
void delay(unsigned int n)
{
  while (n-->0);
}
void clk_HSE()
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV8);
 
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
  
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