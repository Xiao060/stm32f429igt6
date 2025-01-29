#include "stm32f4xx.h"
#include "delay.h"

static uint32_t g_fac_us = 0;       /* us延时倍乘数 */

// 参数表示 每毫秒需要多少个时钟节拍数
void delay_init(uint16_t sysclk) {
    g_fac_us = sysclk;
}

// NOTE: 时钟摘取法进行延时
// HAL_Delay() 函数使用 uwTick 变量, 该变量的修改依赖于系统滴答定时器中断
// delay 函数直接读取 SysTick->VAL, 不依赖额外中断
/*
typedef struct {
    __IOM uint32_t CTRL;
    __IOM uint32_t LOAD;    // VAL 递减到 0 时重新装载的值
    __IOM uint32_t VAL;     // 一个递减的值, 减到 0 时重新装载 LOAD 的值
    __IM  uint32_t CALIB;
} SysTick_Type;
*/
void delay_us(uint32_t us) {
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = us * g_fac_us;                 /* 需要的节拍数 */

    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
}

void delay_ms(uint16_t ms) {
    delay_us((uint32_t)ms * 1000);
}

void HAL_Delay(uint32_t Delay) {
    delay_ms(Delay);
}