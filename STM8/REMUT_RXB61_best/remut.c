
/* Includes ------------------------------------------------------------------*/
#include "remut.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
#define RF                  GPIO_ReadInputPin(GPIOA, GPIO_PIN_1)
remut_t remut;

 void inTimer(){
    if (TIM1_GetITStatus(TIM1_IT_UPDATE) != RESET) 
    {
       
        TIM1_ClearITPendingBit(TIM1_IT_UPDATE); 
      if (RF)
        {
            if (remut.change1)
            {
                ++remut.dataH;
            }
            if (remut.change0)
            {
                remut.H = remut.dataH;
                remut.dataH = 1;
                remut.L = remut.dataL;
                remut.dataL = 0;
                if (remut.start)
                {
                    if (remut.error == 1)
                    {
                        remut.error = 0;
                        remut.start = 0;
                        remut.i = 0;
                    }
                    if (remut.H > remut.L)
                    {
                        remut.n = remut.H / remut.L;
                        if (remut.n > 1 && remut.n < 6)
                        {
                            //remut.data[(remut.i / 8)] |= (1 << (7-remut.i % 8)); // for memory
                            remut.data[remut.i ] = 1; // for time 
                            ++remut.i;
                        }
                        else
                        {
                            remut.error = 1;
                        }
                        if (remut.i > 23)
                        {
                            remut.i = 0;
                            remut.start = 0;
                            remut.read_remut = 1;
                        }
                    }
                    if (remut.L > remut.H)
                    {
                        remut.n = remut.L / remut.H;
                        if (remut.n > 1 && remut.n < 6)
                        {
                            //remut.data[(remut.i / 8)] &= (~(1 << (7-remut.i % 8))); // for memory
                            remut.data[remut.i] = 0; // for time
                            ++remut.i;
                        }
                        else
                        {
                            remut.error = 1;
                        }
                        if (remut.i > 23)
                        {
                            remut.i = 0;
                            remut.start = 0;
                            remut.read_remut = 1;
                        }
                    }
                }
                else if (remut.L > remut.H)
                {
                    remut.n = remut.L / remut.H;
                    if (remut.n > 18 && remut.n < 40)
                    {
                        remut.start = 1;
                        remut.i = 0;
                    }
                }
            }
            remut.change1 = 1;
            remut.change0 = 0;
        }
        if (RF == 0)
        {
            if (remut.change0)
            {
                ++remut.dataL;
            }
            if (remut.change1)
            {
                remut.dataL = 1;
            }
            remut.change0 = 1;
            remut.change1 = 0;
        
    }
}
}

