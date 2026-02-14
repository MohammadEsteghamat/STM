#ifndef __REMUTS_H
#define  __REMUTS_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

typedef struct {
  char change1    :1;
  char change0    :1;
  char start      :1;
  char read_remut :1;
  char error      :1;

  //char data[3]; //for memory
  char data[24];  //for time 
  char i :5;
  char n :6;
  unsigned int dataH;
  unsigned int dataL;
  unsigned int H;
  unsigned int L;
}remut_t;
extern remut_t remut;
inline void inTimer();
#endif /*  __REMUTS_H */



