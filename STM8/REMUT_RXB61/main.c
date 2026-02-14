
#include <stm8s.h>
extern _Bool change1 ;
extern _Bool change0 ;
extern _Bool start ;
extern _Bool read_remut ;
extern _Bool error ;

extern char data[24];
extern char i;
extern char j;
extern char n;
extern unsigned int dataH;
extern unsigned int dataL;
extern unsigned int H;
extern unsigned int L;
char he[3];
void delayq(unsigned int n)
{
  while (n-->0);
}

void bin_to_256(char * a,char * b){
  a[0] = (b[0]-48 << 7)|(b[1]-48 << 6)|(b[2]-48 << 5)|(b[3]-48 << 4)|(b[4]-48 << 3)|(b[5]-48 << 2)|(b[7]-48 << 1)|(b[7]-48 << 0); 
  a[1] = (b[8]-48 << 7)|(b[9]-48 << 6)|(b[10]-48 << 5)|(b[11]-48 << 4)|(b[12]-48 << 3)|(b[13]-48 << 2)|(b[14]-48 << 1)|(b[15]-48 << 0);
  a[2] = (b[16]-48 << 7)|(b[17]-48 << 6)|(b[18]-48 << 5)|(b[19]-48 << 4)|(0 << 3)|(0 << 2)|(0 << 1)|(0 << 0);
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
void delay_ms(unsigned long int n){
    unsigned long int x = 0;
    unsigned long fcpu = 16000000;
    x = fcpu / 1000;
    x = x / 50;
    n = n * x;
    while(n-- > 0){
    }
    
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

    // 4. ????? ????????? (Time Base)
    // ????????: Prescaler, CounterMode, Period, RepetitionCounter
    // Prescaler = 15999 -> ?????? ????? = 16MHz / 16000 = 1000Hz
    // Period = 999 -> ????? ?? 1000 ????? (1 ?????)
    TIM1_TimeBaseInit(39, TIM1_COUNTERMODE_UP, 7, 0); //

    // 5. ????????? ???? ????? (Update Interrupt)
    // ??? ???? ???? ????? ?? ????? ????? ?? ???? ??????
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); //

    // 6. ????????? ???? ?????? (Global Interrupts)
    enableInterrupts();

    // 7. ???? ???? ?????
    TIM1_Cmd(ENABLE); //
    uart_init();
    GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
    delay_ms(10);
     GPIO_WriteLow(GPIOD,GPIO_PIN_4);
    while(1){
      if(read_remut == 1){
        GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
        bin_to_256(he,data);

 
        for(int hj =0;hj<3;hj++)
        {
          
          chap_int(he[hj]);
                           delayq(500);
        }
        UART1_SendData8('R');
        delayq(500);
        UART1_SendData8('\n');
        read_remut = 0;
        delay_ms(10);
        GPIO_WriteLow(GPIOD,GPIO_PIN_4); 
        
      }

    }
    return 0;
}


