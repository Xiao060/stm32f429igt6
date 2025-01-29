#include "stm32f4xx_hal.h"
#include "sys.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"


int main(void) {

    /* 初始化 Hal 库, 主要工作:
        1. 启用 指令缓存/数据缓存/预取缓冲区
        2. 设置中断优先级分组
        3. 使用 SysTick 作为时间基准, 配置 1ms 的滴答中断
            NOTE: 在系统复位后, 系统时钟使用默认的内部高速时钟(HSI)
        4. 初始化低级硬件
    */
    HAL_StatusTypeDef ret = HAL_Init();
    if (ret != HAL_OK) {
        return 1;
    }
    // 配置 RCC, 设置系统时钟源使用 HSE
    sys_clock_init(25, 360, 2, 8);
    // 参数: 系统时钟频率, 单位 Mhz
    delay_init(180);
    usart_init(115200);

    while(1) {
        // delay_ms(1000);
        if (RX_FLAG_IS(RX_FLAG_0A)) {
            printf("Receive Data: %s\r\n", g_usart_rx_buf);
            memset(g_usart_rx_buf, 0, USART_REC_LEN);
            // HAL_UART_Transmit(&g_huart, (uint8_t*)"Receive Data: ", 15, 1000);
            // HAL_UART_Transmit(&g_huart, g_usart_rx_buf, g_rx_idx, 1000);
            g_rx_idx = 0;
            RX_FLAG_SET(RX_FLAG_0);
        }
    }

    return 0;
}