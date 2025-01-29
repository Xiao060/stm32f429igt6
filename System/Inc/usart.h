#ifndef __USART_H
#define __USART_H

#include <stdint.h>

#define USART_REC_LEN   200      // 定义最大接收字节数 200
#define USART_EN_RX     1        // 使能（1）/禁止（0）串口1接收
#define RX_BUFFER_SIZE  1        // 缓存大小

extern UART_HandleTypeDef g_huart;
extern uint8_t g_usart_rx_buf[USART_REC_LEN];

// 接收长度/标志位
extern uint8_t g_rx_idx;
extern uint8_t g_rx_flag;
#define RX_FLAG_0       0x1
#define RX_FLAG_0D      0x2         // \r
#define RX_FLAG_0A      0x4         // \n

#define RX_FLAG_IS(flag)   ((g_rx_flag & flag) == flag)
#define RX_FLAG_SET(flag)  (g_rx_flag = flag)

void usart_init(uint32_t baudrate);

#endif