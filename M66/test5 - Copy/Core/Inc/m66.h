#ifndef _M66_H_
#define _M66_H_

#include "main.h"

// === SMS Buffers ===
extern char m66_sms_sender_number[20];
extern char m66_sms_timestamp[25];
extern char m66_sms_inbox_message[161];
extern char m66_uart_rx_buffer[315];

// === Flags ===
extern volatile _Bool m66_sms_received_flag;

// === Function Prototypes ===
int  m66_init(UART_HandleTypeDef *huart);
int  m66_sms_send(const char *message, const char *phone);
void m66_uart_start_receive(void);
void m66_uart_stop_receive(void);
void m66_sms_read(void);
void prepare_receive_buffer(void);
void m66_sms_send_farsi_full(const char *phone_ucs2, const uint8_t *message_ucs2, size_t message_len);
#endif
