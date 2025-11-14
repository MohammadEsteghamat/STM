#ifndef _M66_
#define _M66_
#include "main.h"
extern char SMS_Phone[32];
extern char SMS_Date[32];
extern char Received_SMS[241];
extern char rx[300];
extern _Bool flg;
int m66_init(UART_HandleTypeDef *huart);
int send_sms(const char *message, const char *phone);
void m66_start_receive(void);
void get_sms(void);
#endif

