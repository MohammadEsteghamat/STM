/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint16_t i;
void delay_ms(unsigned long int n){
  unsigned long int x = 0;
  unsigned long fcpu = 16000000;
  x = fcpu / 1000;
  x = x / 50;
  n = n * x;
  while(n-- > 0){} 
}
void clk_HSE();
void pwm_init();
void main(void)
{
  /* Infinite loop */
  clk_HSE();

  pwm_init();
  while (1)
  {
    for(i =0;i<1024;i++){
    TIM2_SetCompare1(2);
    delay_ms(200);
    }
  }
  
}
void clk_HSE()
{
  CLK_HSECmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2,ENABLE);

}

void pwm_init()
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_8,1000);
  TIM2_OC2Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,1000,TIM2_OCPOLARITY_HIGH);
  TIM2_Cmd(ENABLE);


}

