#include "stm32f4xx.h"
#include "usart.h"


// 临时接收缓冲区, 用于从中断中接收串口数据
// 此处长度设为 1B 是因为需要检测接收到的数据是否为 \r \n
uint8_t g_rx_buffer[RX_BUFFER_SIZE];

// 全局接收缓冲区, 将临时接收缓冲区的数据拷贝到该缓冲区进行组包
uint8_t g_usart_rx_buf[USART_REC_LEN];
// 接收标志位
uint8_t g_rx_idx = 0;
uint8_t g_rx_flag = RX_FLAG_0;


// 该数据地址需要作为参数传入 HAL_UART_Init/HAL_UART_IRQHandler
// 故需要设置为全局变量
UART_HandleTypeDef g_huart;

void usart_init(uint32_t baudrate) {
    g_huart.Instance = USART1;
    g_huart.Init.BaudRate = baudrate;
    g_huart.Init.WordLength = UART_WORDLENGTH_8B;
    g_huart.Init.StopBits = UART_STOPBITS_1;
    g_huart.Init.Parity = UART_PARITY_NONE;         // 无校验
    g_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;   // 无硬件流控
    g_huart.Init.Mode = UART_MODE_TX_RX;            // 收发模式
    // HAL_UART_Init 会调用 HAL_UART_MspInit 去初始化底层硬件
    // HAL_UART_MspInit 默认为空函数, 需要重写该函数
    HAL_UART_Init(&g_huart);

    HAL_UART_Receive_IT(&g_huart, (uint8_t*)g_rx_buffer, RX_BUFFER_SIZE);
}

// HAL_UART_MspInit 默认为空函数, 需要重写该函数
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

    // 使用 GPIOA 的 PIN9 作为接收引脚, PIN10 作为发送引脚

    // USART1 时钟使能
    __HAL_RCC_USART1_CLK_ENABLE();
    // 接收/发送引脚时钟使能 ? 一样的操作
    __HAL_RCC_GPIOA_CLK_ENABLE();
    // __HAL_RCC_GPIOA_CLK_ENABLE();

    // 接收引脚初始化, GPIOA 的 PIN9
    GPIO_InitTypeDef gpio_init = {
        .Pin = GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,        // 推挽式复用
        .Pull = GPIO_PULLUP,            // 上下拉选择: 上拉
        .Speed = GPIO_SPEED_FREQ_HIGH,  // 快速
        .Alternate = GPIO_AF7_USART1    // 所选引脚设置: 复用为 USART1
    };
    HAL_GPIO_Init(GPIOA, &gpio_init);

    // 发送引脚初始化, GPIOA 的 PIN10
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    // 使能 USART1 的中断
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    // 设置 USART1 的中断优先级, 抢占优先级 3, 子优先级 3
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
}

// 串口中断处理函数 USART1_IRQHandler 默认为 Default_Handler
// 需要重写完成想要的功能
void USART1_IRQHandler(void) {
    // 重写 USART 中断处理函数可以直接调用 HAL_UART_IRQHandler
    HAL_UART_IRQHandler(&g_huart);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    g_usart_rx_buf[g_rx_idx++] = g_rx_buffer[0];
    if (RX_FLAG_IS(RX_FLAG_0) && g_rx_buffer[0] == '\r') {
        RX_FLAG_SET(RX_FLAG_0D);
    } else if (RX_FLAG_IS(RX_FLAG_0D) && g_rx_buffer[0] == '\n') {
        RX_FLAG_SET(RX_FLAG_0A);
    }

    HAL_UART_Receive_IT(huart, (uint8_t*)g_rx_buffer, RX_BUFFER_SIZE);
}