/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "icons.h"
#include "shahid.h"
#include "stdbool.h"
#include "stdio.h"
#include "tft.h"
#define abs(x) ((x) < 0 ? -(x) : (x))
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct {
  double r, g, b;
  char temp[10];
  bool sw1, sw2, sw3, sw4;
} var;
var values = {5, 5, 5, "          ", false, false, false, false};
typedef struct {
  int hours;
  int minutes;
  int seconds;
  int millis;
} tim;
tim time = {0, 0, 0, 0};
bool mode_timer = true;
int menu = 0;
// 0 : pass
// 1 : main menu
// 2 : timer
// 3 : pwm
// 4 : setting
// 5 : theme
// 6 : change
// 7 : set time
int password[4] = {1, 2, 1, 2};
int pass_in[4];
char displayedPassword[4] = "****";
int index_in = 0;
int duty = 50;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
HAL_GPIO_TogglePin(led_GPIO_Port,led_Pin);
  HAL_Delay(100);
  if (GPIO_Pin == GPIO_PIN_3) {
    values.sw1 = true;
		
  } else if (GPIO_Pin == GPIO_PIN_4) {
    values.sw2 = true;
  } else if (GPIO_Pin == GPIO_PIN_5) {
    values.sw3 = true;
  } else if (GPIO_Pin == GPIO_PIN_6) {
    values.sw4 = true;
  }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == &htim2) {
    time.millis++;
  }
  if (mode_timer == 1) {
    if (time.millis >= 1000) {
      time.seconds += time.millis / 1000;
      time.millis %= 1000;
      if (menu == 2) {
        LCD_Fill(170, 60, 370, 100,
                 change_color(LightBlue, values.r, values.g, values.b));
        uint16_t t = BACK_COLOR;
        LCD_SetBackColor(change_color(LightBlue, values.r, values.g, values.b));
        sprintf(values.temp, "%02d:%02d:%02d", time.hours, time.minutes,
                time.seconds);
        LCD_ShowString(170, 60, 400, 240, 18, (uint8_t *)values.temp);
        LCD_SetBackColor(t);
      }
    }
    if (time.seconds >= 60) {
      time.minutes += time.seconds / 60;
      time.seconds %= 60;
    }
    if (time.minutes >= 60) {
      time.hours += time.minutes / 60;
      time.minutes %= 60;
    }
  } else if (mode_timer == 0) {
    if (time.millis >= 1000) {
      time.seconds -= time.millis / 1000;
      time.millis %= 1000;
      if (menu == 2) {
        LCD_Fill(170, 60, 370, 100,
                 change_color(LightBlue, values.r, values.g, values.b));
        uint16_t t = BACK_COLOR;
        LCD_SetBackColor(change_color(LightBlue, values.r, values.g, values.b));
        sprintf(values.temp, "%02d:%02d:%02d", time.hours, time.minutes,
                time.seconds);
        LCD_ShowString(170, 60, 400, 240, 18, (uint8_t *)values.temp);
        LCD_SetBackColor(t);
      }
    }

  
    if (time.seconds < 0) {
      int borrow = (abs(time.seconds) + 59) / 60; 
      time.minutes -= borrow;
      time.seconds += borrow * 60;
    }

    
    if (time.minutes < 0) {
      int borrow = (abs(time.minutes) + 59) / 60; 
      time.hours -= borrow;
      time.minutes += borrow * 60;
    }

    
    if (time.hours <= 0 && time.minutes <= 0 && time.seconds <= 0) {
      time.hours = 0;
      time.minutes = 0;
      time.seconds = 0;
      HAL_TIM_Base_Stop(&htim2);
      mode_timer = 1;
      LCD_ShowString(170, 60, 400, 240, 18, "00:00:00");
    }
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LCD_Init();

  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (menu == 0) {
      // password
      LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
      LCD_Show_Image(0, 0, 99, 99, behesht, values.r, values.g, values.b);
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(105, 30, 400, 240, 26, "MicroProcessor Lab");
      LCD_ShowString(125, 60, 400, 240, 18, "Moahammad Esteghamat");
      LCD_ShowString(125, 90, 400, 240, 18, "Moahmmad Aleali");
      LCD_ShowString(125, 120, 400, 240, 18, "Mandana Shabanzadeh");
      LCD_ShowString(125, 160, 400, 240, 18, "Enter Password:");

      int t = 0;
      while (true) {
        if (values.sw1 == true) {
          if (index_in > 0) {
            displayedPassword[index_in - 1] = '*';
          }
          values.sw1 = false;
          pass_in[index_in] = 1;
          displayedPassword[index_in] = '1';
          index_in++;

        } else if (values.sw2 == true) {
          if (index_in > 0) {
            displayedPassword[index_in - 1] = '*';
          }
          values.sw2 = false;
          pass_in[index_in] = 2;
          displayedPassword[index_in] = '2';
          index_in++;

        } else if (values.sw3 == true) {
          if (index_in > 0) {
            displayedPassword[index_in - 1] = '*';
          }
          values.sw3 = false;
          pass_in[index_in] = 3;
          displayedPassword[index_in] = '3';
          index_in++;

        } else if (values.sw4 == true) {
          if (index_in > 0) {
            displayedPassword[index_in - 1] = '*';
          }
          values.sw4 = false;
          pass_in[index_in] = 4;
          displayedPassword[index_in] = '4';
          index_in++;
        }
        LCD_ShowString(125, 180, 400, 240, 26, (uint8_t *)displayedPassword);
        if (index_in == 4) {
          HAL_Delay(100);
          index_in = 0;
          displayedPassword[0] = '*';
          displayedPassword[1] = '*';
          displayedPassword[2] = '*';
          displayedPassword[3] = '*';

          if (password[0] == pass_in[0] && password[1] == pass_in[1] &&
              password[2] == pass_in[2] && password[3] == pass_in[3]) {
            menu = 1;
            index_in = 0;
            displayedPassword[0] = '*';
            displayedPassword[1] = '*';
            displayedPassword[2] = '*';
            displayedPassword[3] = '*';

            break;
          }
        }

        t++;
        if ((t - 100) == 0) {
          t = 0;
          displayedPassword[0] = '*';
          displayedPassword[1] = '*';
          displayedPassword[2] = '*';
          displayedPassword[3] = '*';
        }
        HAL_Delay(10);
      }
    }
    else if (menu == 1) {
      LCD_Fill(0, 0, 399, 20,
               change_color(WHITE, values.r, values.g, values.b));
      LCD_Fill(0, 20, 200, 130,
               change_color(LightBlue, values.r, values.g, values.b));
      LCD_Fill(200, 20, 399, 130,
               change_color(LightPeach, values.r, values.g, values.b));
      LCD_Fill(0, 130, 200, 239,
               change_color(LightYellow, values.r, values.g, values.b));
      LCD_Fill(200, 130, 399, 239,
               change_color(LightGrey, values.r, values.g, values.b));
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(150, 0, 400, 240, 18, "Main Menu");
      LCD_SetBackColor(change_color(LightBlue, values.r, values.g, values.b));
      LCD_ShowString(75, 65, 400, 240, 18, "1-Timer");
      LCD_SetBackColor(change_color(LightPeach, values.r, values.g, values.b));
      LCD_ShowString(275, 65, 400, 240, 18, "2-PWM");
      LCD_SetBackColor(change_color(LightYellow, values.r, values.g, values.b));
      LCD_ShowString(55, 175, 400, 240, 18, "3-Settings");
      LCD_SetBackColor(change_color(LightGrey, values.r, values.g, values.b));
      LCD_ShowString(275, 175, 400, 240, 18, "4-Back");
      while (true) {
        if (values.sw1 == true) {
          values.sw1 = false;
          menu = 2;
          break;
        } else if (values.sw2 == true) {
          values.sw2 = false;
          menu = 3;
          break;
        } else if (values.sw3 == true) {
          values.sw3 = false;
          menu = 4;
          break;
        } else if (values.sw4 == true) {
          values.sw4 = false;
          menu = 0;
          break;
        }
        HAL_Delay(10);
      }
    } 
    else if (menu == 2) {
      int sel = 3;
      LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(145, 5, 400, 240, 18, "stop watch timer");
      LCD_Fill(30, 30, 370, 100,
               change_color(LightBlue, values.r, values.g, values.b));

      LCD_Show_Image(30 , 110, 50, 50, play, values.r, values.g, values.b);
      LCD_Show_Image(127, 110, 50, 50, pause, values.r, values.g,
                     values.b);
      LCD_Show_Image(224 , 110, 50, 50, Restart, values.r, values.g,
                     values.b);
      LCD_Show_Image(321 , 110, 50, 50, set_time, values.r, values.g,
                     values.b);

      LCD_ShowString(30, 180, 400, 240, 18,
                     "ok : push button 1   Back:push button 4");
      LCD_ShowString(20, 210, 400, 240, 18,
                     " < : push button 2     > :push button 3");


      while (true) {

        if (values.sw1 == true) {
          values.sw1 = false;
          if (sel == 0) {
            HAL_TIM_Base_Start_IT(&htim2);
          } 
					else if (sel == 1) {
            HAL_TIM_Base_Stop_IT(&htim2);
          }
					else if (sel == 2) {
            time.hours = 0;
            time.minutes = 0;
            time.seconds = 0;
            time.millis = 0;
            sprintf(values.temp, "%02d:%02d:%02d", time.hours, time.minutes,
                    time.seconds);
            uint16_t t = BACK_COLOR;
            LCD_SetBackColor(
                change_color(LightBlue, values.r, values.g, values.b));
            LCD_ShowString(170, 60, 400, 240, 18, (uint8_t *)values.temp);
            LCD_SetBackColor(t);
          } 
					else if (sel == 3) {
            menu = 7;
            break;
          }
        } else if (values.sw2 == true) {
          values.sw2 = false;
          for (int i = 0; i < 5; i++) {
            Draw_Circle(55  + 97 * sel, 130, 30 + i, WHITE);
          }
          sel++;
          if (sel > 3) {
            sel = 0;
          }
          for (int i = 0; i < 5; i++) {
            Draw_Circle(55  + 97 * sel, 130, 30 + i, Black);
          }

        } else if (values.sw3 == true) {
          values.sw3 = false;
          for (int i = 0; i < 5; i++) {
            Draw_Circle(55  + 97 * sel, 130, 30 + i, WHITE);
          }
          sel--;
          if (sel < 0) {
            sel = 3;
          }
          for (int i = 0; i < 5; i++) {
            Draw_Circle(55  + 97 * sel, 130, 30 + i, Black);
          }

        } else if (values.sw4 == true) {
          values.sw4 = false;
          menu = 1;
          break;
        }

        HAL_Delay(10);
      }

    } 
    else if (menu == 3) {
      LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(140, 2, 400, 240, 18, "PWM");
      int sel = 0;
      bool fsel = true;
      bool flag = true;
      bool Working = true;
      while (true) {
        if (values.sw1 == true) {
          values.sw1 = false;
          if (sel == 1) {
            Working = true;
            fsel = true;
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
          } else if (sel == 0) {
            duty++;
            flag = true;
            TIM1->CCR1 = duty * 10;
          }
        } else if (values.sw2 == true) {
          values.sw2 = false;
          if (sel == 1) {
            Working = false;
            fsel = true;
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
          } else if (sel == 0) {
            duty--;
            flag = true;
            TIM1->CCR1 = duty * 10;
          }
        } else if (values.sw3 == true) {
          values.sw3 = false;
          sel = (sel + 1) % 2;
          fsel = true;
        } else if (values.sw4 == true) {
          values.sw4 = false;
          menu = 1;
          break;
        }

        if (sel == 0 && (fsel || flag)) {
          fsel = false;
          LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
          LCD_ShowString(30, 140, 400, 240, 26, "             ");
          LCD_ShowString(30, 100, 400, 240, 26, "                          ");
          LCD_ShowString(30, 100, 400, 240, 26, "PWM Duty Cycle:");
          sprintf(values.temp, "%3d", duty);
          LCD_ShowString(270, 110, 400, 240, 18, (uint8_t *)values.temp);
          if (Working) {
            LCD_ShowString(30, 140, 400, 240, 18, "PWM : ON");
          } else {
            LCD_ShowString(30, 140, 400, 240, 18, "PWM : OFF");
          }

        } else if (sel == 1 && (fsel || flag)) {
          fsel = false;
          LCD_ShowString(30, 140, 400, 240, 26, "              ");
          LCD_ShowString(30, 100, 400, 240, 26, "                           ");
          LCD_ShowString(30, 100, 400, 240, 18, "PWM Duty Cycle:");
          sprintf(values.temp, "%3d", duty);
          LCD_ShowString(140, 100, 400, 240, 18, (uint8_t *)values.temp);
          if (Working) {
            LCD_ShowString(30, 140, 400, 240, 26, "PWM : ON");
          } else {
            LCD_ShowString(30, 140, 400, 240, 26, "PWM : OFF");
          }
        }
        if (flag) {
          flag = false;
          LCD_Fill(20, 20, 320, 60,
                   change_color(WHITE, values.r, values.g, values.b));

          for (int i = 0; i < 3; i++) {
            LCD_DrawLine(20 + i * 100, 60, 20 + i * 100, 20);
            LCD_DrawLine(20 + i * 100, 20, 20 + duty + i * 100, 20);
            LCD_DrawLine(20 + duty + i * 100, 20, 20 + duty + i * 100, 60);
            LCD_DrawLine(20 + duty + i * 100, 60, 20 + (i + 1) * 100, 60);
          }
        }
      }

    } 
    else if (menu == 4) {
      LCD_Fill(0, 0, 399, 20,
               change_color(WHITE, values.r, values.g, values.b));
      LCD_Fill(0, 20, 200, 130,
               change_color(LightBlue, values.r, values.g, values.b));
      LCD_Fill(200, 20, 399, 130,
               change_color(LightPeach, values.r, values.g, values.b));
      LCD_Fill(0, 130, 399, 239,
               change_color(LightGrey, values.r, values.g, values.b));
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(165, 0, 400, 240, 18, "settings");
      LCD_SetBackColor(change_color(LightBlue, values.r, values.g, values.b));
      LCD_ShowString(20, 65, 400, 240, 18, "1-Change Password");
      LCD_SetBackColor(change_color(LightPeach, values.r, values.g, values.b));
      LCD_ShowString(265, 65, 400, 240, 18, "2-Theme");
      LCD_SetBackColor(change_color(LightGrey, values.r, values.g, values.b));
      LCD_ShowString(170, 175, 400, 240, 18, "4-Back");
      while (true) {
        if (values.sw1 == true) {
          values.sw1 = false;
          menu = 6;
          break;
        } else if (values.sw2 == true) {
          values.sw2 = false;
          menu = 5;
          break;
        } else if (values.sw3 == true) {
          values.sw3 = false;
        } else if (values.sw4 == true) {
          values.sw4 = false;
          menu = 1;
          break;
        }
        HAL_Delay(10);
      }
    } 
    else if (menu == 5) {
      int sel = 0;
      bool fsel = true;
      bool fal = false;
      LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(180, 5, 400, 240, 18, "Theme");
      LCD_ShowString(0, 220, 400, 240, 18,
                     "1- INCREASE 2- DECREASE 3- SELECT 4- BACK");
      while (true) {
        if (values.sw1 == true) {
          values.sw1 = false;
          if (sel == 0) {
            values.r++;
          } else if (sel == 1) {
            values.g++;
          } else if (sel == 2) {
            values.b++;
          }
          fal = true;
        } else if (values.sw2 == true) {
          values.sw2 = false;
          if (sel == 0) {
            values.r--;
          } else if (sel == 1) {
            values.g--;
          } else if (sel == 2) {
            values.b--;
          }
          fal = true;
        } else if (values.sw3 == true) {
          values.sw3 = false;
          sel = (sel + 1) % 3;
          fsel = true;
        } else if (values.sw4 == true) {
          values.sw4 = false;
          menu = 4;
          POINT_COLOR = Black;
          break;
        }

        if (values.r < 0) {
          values.r = 0;
        }
        if (values.r > 20) {
          values.r = 20;
        }

        if (values.g < 0) {
          values.g = 0;
        }
        if (values.g > 20) {
          values.g = 20;
        }

        if (values.b < 0) {
          values.b = 0;
        }
        if (values.b > 20) {
          values.b = 20;
        }

        POINT_COLOR = RED;
        LCD_DrawRectangle(100, 50, 300, 90);
        POINT_COLOR = Green;
        LCD_DrawRectangle(100, 110, 300, 150);
        POINT_COLOR = Blue;
        LCD_DrawRectangle(100, 170, 300, 210);
        POINT_COLOR = Black;

        if (sel == 0 && fsel) {
          LCD_ShowString(15, 170, 400, 240, 26, "    ");
          LCD_ShowString(15, 50, 400, 240, 26, "   ");
          LCD_ShowString(15, 110, 400, 240, 26, "     ");
          POINT_COLOR = RED;
          LCD_ShowString(15, 50, 400, 240, 26, "RED");
          POINT_COLOR = Green;
          LCD_ShowString(15, 110, 400, 240, 18, "GREEN");
          POINT_COLOR = Blue;
          LCD_ShowString(15, 170, 400, 240, 18, "BLUE");
          fsel = false;
        } else if (sel == 1 && fsel) {
          LCD_ShowString(15, 170, 400, 240, 26, "    ");
          LCD_ShowString(15, 50, 400, 240, 26, "   ");
          LCD_ShowString(15, 110, 400, 240, 26, "     ");
          POINT_COLOR = Green;
          LCD_ShowString(15, 110, 400, 240, 26, "GREEN");
          POINT_COLOR = RED;
          LCD_ShowString(15, 50, 400, 240, 18, "RED");
          POINT_COLOR = Blue;
          LCD_ShowString(15, 170, 400, 240, 18, "BLUE");
          fsel = false;
        } else if (sel == 2 && fsel) {
          LCD_ShowString(15, 170, 400, 240, 26, "    ");
          LCD_ShowString(15, 50, 400, 240, 26, "   ");
          LCD_ShowString(15, 110, 400, 240, 26, "     ");
          POINT_COLOR = Blue;
          LCD_ShowString(15, 170, 400, 240, 26, "BLUE");
          POINT_COLOR = RED;
          LCD_ShowString(15, 50, 400, 240, 18, "RED");
          POINT_COLOR = Green;
          LCD_ShowString(15, 110, 400, 240, 18, "GREEN");
          fsel = false;
        }

        POINT_COLOR = RED;
        for (int i = 0; i < 5 * values.r; i++) {
          LCD_DrawLine(100 + i, 50, 100 + i, 90);
        }
        POINT_COLOR = Green;
        for (int i = 0; i < 5 * values.g; i++) {
          LCD_DrawLine(100 + i, 110, 100 + i, 150);
        }
        POINT_COLOR = BLUE;
        for (int i = 0; i < 5 * values.b; i++) {
          LCD_DrawLine(100 + i, 170, 100 + i, 210);
        }
        if (fal) {
          fal = false;
          LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
          LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
          LCD_ShowString(180, 5, 400, 240, 18, "Theme");
          LCD_ShowString(0, 220, 400, 240, 18,
                         "1- INCREASE 2- DECREASE 3- SELECT 4- BACK");
          fsel = true;
        }
        HAL_Delay(10);
      }
    } 
    else if (menu == 6) {
      LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(80, 5, 400, 240, 26, "Change Password");
      LCD_ShowString(30, 60, 400, 240, 26, "Login password :");
      while (true) {
        if (values.sw1 == true) {
          values.sw1 = false;
          displayedPassword[index_in] = '1';
					pass_in[index_in] = 1;
          index_in++;

        } else if (values.sw2 == true) {
          values.sw2 = false;
          displayedPassword[index_in] = '2';
					pass_in[index_in] = 2;
          index_in++;

        } else if (values.sw3 == true) {
          values.sw3 = false;
          displayedPassword[index_in] = '3';
					pass_in[index_in] = 3;
          index_in++;

        } else if (values.sw4 == true) {
          values.sw4 = false;
          displayedPassword[index_in] = '4';
					pass_in[index_in] = 4;
          index_in++;
        }

        LCD_ShowString(280, 60, 400, 240, 26, (uint8_t *)displayedPassword);
        if (index_in == 4) {
          index_in = 0;
          menu = 4;
          displayedPassword[0] = '*';
          displayedPassword[1] = '*';
          displayedPassword[2] = '*';
          displayedPassword[3] = '*';
          LCD_ShowString(30, 160, 400, 229, 18,
                         "The password has been changed");
					for (int i = 0; i < 4; i++) {
						password[i] = pass_in[i] ;
					}
          HAL_Delay(100);
          break;
        }
        HAL_Delay(10);
      }
    } 
    else if (menu == 7) {
      LCD_Clear(change_color(WHITE, values.r, values.g, values.b));
      LCD_SetBackColor(change_color(WHITE, values.r, values.g, values.b));
      LCD_ShowString(180, 5, 400, 240, 18, "set time");
      tim temp = {0, 0, 0, 0};
      int speed = 1;
      int sel = 0;
 LCD_Fill(30, 30, 370, 100,
               change_color(LightBlue, values.r, values.g, values.b));
      while (true) {
        if (values.sw1 == true) {
          values.sw1 = false;
          speed++;

        } else if (values.sw2 == true) {
          values.sw2 = false;
          speed--;
        } else if (values.sw3 == true) {
          values.sw3 = false;
          sel = (sel + 1) % 3;
        } else if (values.sw4 == true) {
          values.sw4 = false;
          menu = 2;
          time.millis = temp.millis;
          time.seconds = temp.seconds;
          time.minutes = temp.minutes;
          time.hours = temp.hours;
          mode_timer = 0;
          break;
        }
        if (sel == 2) {
          temp.seconds = (temp.seconds + speed) % 60;
          if (temp.seconds < 0) {
            temp.seconds = 0;
          }

        } else if (sel == 1) {
          temp.minutes = (temp.minutes + speed) % 60;
          if (temp.minutes < 0) {
            temp.minutes = 0;
          }
        } else if (sel == 0) {
          temp.hours = (temp.hours + speed) % 60;
          if (temp.hours < 0) {
            temp.hours = 0;
          }
        }
        for (int i = 0; i < 4; i++) {
					POINT_COLOR = change_color(Black, values.r, values.g, values.b);
          LCD_DrawLine(180 + sel * 25, 80, 190 + sel * 25, 80);
          HAL_Delay(50);
					POINT_COLOR = change_color(LightBlue, values.r, values.g, values.b);
          LCD_DrawLine(180 + sel * 25, 80, 190 + sel * 25, 80);
          HAL_Delay(50);
        }
				POINT_COLOR = 0x0000;
        sprintf(values.temp, "%2d:%2d:%2d", temp.hours, temp.minutes,
                temp.seconds);
        LCD_SetBackColor(change_color(LightBlue, values.r, values.g, values.b));
        LCD_ShowString(170, 60, 400, 240, 18, (uint8_t *)values.temp);
      }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
