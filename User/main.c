#include "stm32f4xx_hal.h"
#include "sys.h"

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
    // 配置 RCC, 设置最终的系统时钟源
    sys_clock_init(25, 360, 2, 8);

    while(1) {

    }

    return 0;
}