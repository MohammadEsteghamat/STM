#include <m66.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define MSG_TO_DETECT_SMS "\r\n+CMTI: \"SM\",1\r\n"
#define BUF_SIZE 17
volatile _Bool m66_sms_received_flag = 0;
static const char CTRL_Z = 0x1A;
static UART_HandleTypeDef *huart_m66;
static uint8_t MAX_RETRY = 5; 
char m66_sms_timestamp[25] = {0};
char m66_sms_sender_number[20] = {0};
char m66_sms_inbox_message[161] = {0};
char m66_uart_rx_buffer[315] = {0};
static uint8_t m66_uart_rx_buffer_byte;
static uint8_t m66_uart_rx_buffer_buffer[BUF_SIZE];
static uint8_t m66_uart_rx_buffer_index = 0;
int waitForResponseRetry(const char *expected, uint32_t timeout) {
    uint8_t ch;
    size_t len = strlen(expected);
    char buffer[20] = {0};
    if(len > sizeof(buffer)) return 0; 
    size_t i = 0;
    int retry = 0;

    while (retry < MAX_RETRY) {
        uint32_t tickstart = HAL_GetTick();
        i = 0;
        memset(buffer, 0, sizeof(buffer));

        while ((HAL_GetTick() - tickstart) < timeout) {
            if (HAL_UART_Receive(huart_m66, &ch, 1, 100) == HAL_OK) {
                buffer[i % len] = ch;
                i++;

                if (i >= len) {
                    int matched = 1;
                    for (size_t j = 0; j < len; j++) {
                        if (buffer[(i - len + j) % len] != expected[j]) {
                            matched = 0;
                            break;
                        }
                    }
                    if (matched) return 1; 
                }
            }
        }
        retry++;
    }
    return 0; 
}

int m66_init(UART_HandleTypeDef *huart) {
    huart_m66 = huart;
    char buffer[64]; 

    snprintf(buffer, sizeof(buffer), "ATE0\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;

    snprintf(buffer, sizeof(buffer), "AT+CFUN=1\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 2000)) return 0;

    snprintf(buffer, sizeof(buffer), "AT+CMGF=1\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;

    snprintf(buffer, sizeof(buffer), "AT+CSCS=\"GSM\"\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;

    snprintf(buffer, sizeof(buffer), "AT+CSMP=17,167,0,0\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;
		//-----------------------------m66_sms_read--------------------------------------
		snprintf(buffer, sizeof(buffer), "AT+CMGF=1\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;

    snprintf(buffer, sizeof(buffer), "AT+CNMI=2,1,0,0,0\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;
  
    snprintf(buffer, sizeof(buffer), "AT+CMGD=1,4\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return 0;

    return 1;
}

int m66_sms_send(const char *message, const char *phone) {
    char buffer[300];
    size_t len;

    if (message == NULL || phone == NULL) return 0;

    snprintf(buffer, sizeof(buffer), "AT+CMGS=\"%s\"\r\n", phone);
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);

    if (!waitForResponseRetry(">", 1000)) return 0;

    len = strlen(message);
    if (len > (sizeof(buffer) - 10)) return 0;

    snprintf(buffer, sizeof(buffer), "%s", message);
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);

    HAL_UART_Transmit(huart_m66, (uint8_t*)&CTRL_Z, 1, 100);

    if (!waitForResponseRetry("+CMGS:", 5000)) return 0;
    if (!waitForResponseRetry("\r\nOK\r\n", 2000)) return 0;

    return 1;
}


void m66_sms_read(void) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "AT+CMGR=1\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);

    // clear m66
	  prepare_receive_buffer();
    
    int idx = 0;
    uint8_t ch;

    
    while (1) {
        if (HAL_UART_Receive(huart_m66, &ch, 1, 100) == HAL_OK) {
            if (idx < (int)sizeof(m66_uart_rx_buffer)-1) {
                m66_uart_rx_buffer[idx++] = ch;
                m66_uart_rx_buffer[idx] = '\0';
            }
            if (idx >= 4 && strcmp(&m66_uart_rx_buffer[idx-4], "OK\r\n") == 0) break;
						if (idx > 159) break;
        }
    }

    

    sscanf(m66_uart_rx_buffer, "%*[^+]+CMGR: \"%*[^\\\"]\",\"%[^\"]\",\"\",\"%[^\"]\"\r\n%[^\r]\r\nOK\r\n", m66_sms_sender_number, m66_sms_timestamp,m66_sms_inbox_message);
		sprintf(buffer,"AT+CMGD=1,4\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 100);
    if (!waitForResponseRetry("OK\r\n", 1000)) return;
   
}

void m66_uart_start_receive(void) {
    HAL_UART_Receive_IT(huart_m66, &m66_uart_rx_buffer_byte, 1);
}

void m66_uart_stop_receive(void) {
	HAL_UART_AbortReceive_IT(huart_m66);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	
    if (huart == huart_m66) {
		
        m66_uart_rx_buffer_buffer[m66_uart_rx_buffer_index++] = m66_uart_rx_buffer_byte;
        if (m66_uart_rx_buffer_index >= BUF_SIZE) m66_uart_rx_buffer_index = 0;

        int len = strlen(MSG_TO_DETECT_SMS);
        for (int i = 0; i < BUF_SIZE; i++) {
            int match = 1;
            for (int j = 0; j < len; j++) {
                if (m66_uart_rx_buffer_buffer[(i + j) % BUF_SIZE] != MSG_TO_DETECT_SMS[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                m66_sms_received_flag = 1;
                break;
            }
        }

        if(m66_sms_received_flag == 0)  HAL_UART_Receive_IT(huart_m66, &m66_uart_rx_buffer_byte, 1);
    }
}
//uint8_t my_strlen(const char *str)
//{
//    uint8_t len = 0;

//    
//    while (str[len] != '\0')
//    {
//        len++;
//    }

//    return len;
//}
void prepare_receive_buffer(void)
{
	memset(m66_sms_inbox_message, 0, sizeof(m66_sms_inbox_message));
	memset(m66_sms_timestamp, 0, sizeof(m66_sms_timestamp));
	memset(m66_sms_sender_number, 0, sizeof(m66_sms_sender_number));
	memset(m66_uart_rx_buffer, 0, sizeof(m66_uart_rx_buffer));
}

void m66_sms_send_farsi_full(const char *phone_ucs2, const char *message_ucs2_hex)
{
    char buffer[64];
    const char CTRL_Z = 0x1A;

    snprintf(buffer, sizeof(buffer), "AT+CMGF=1\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 1000);
    HAL_Delay(100);

    snprintf(buffer, sizeof(buffer), "AT+CSCS=\"UCS2\"\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 1000);
    HAL_Delay(100);

    snprintf(buffer, sizeof(buffer), "AT+CSMP=17,167,0,0\r\n");
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 1000);
    HAL_Delay(100);

    // ????? ???? UCS2
    snprintf(buffer, sizeof(buffer), "AT+CMGS=\"%s\"\r\n", phone_ucs2);
    HAL_UART_Transmit(huart_m66, (uint8_t*)buffer, strlen(buffer), 1000);

    // **????? ???? ????? prompt > ?????**
    if (!waitForResponseRetry(">", 2000)) return;

    // ??? UCS2 HEX
    HAL_UART_Transmit(huart_m66, (uint8_t*)message_ucs2_hex, strlen(message_ucs2_hex), 1000);

    // CTRL+Z ???? ?????
    HAL_UART_Transmit(huart_m66, (uint8_t*)&CTRL_Z, 1, 1000);

    // ?????? ???? OK
    waitForResponseRetry("OK\r\n", 5000);
}
