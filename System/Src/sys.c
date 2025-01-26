// #include "stm32f4xx_hal_pwr_ex.h"
// #include "stm32f4xx_hal_rcc.h"
#include "sys.h"
#include <stdint.h>



void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset) {
    // 设置NVIC的向量表偏移寄存器, VTOR低9位保留, 即[8:0]保留
    // SCB 系统控制块
    SCB->VTOR = baseaddr | (offset & (uint32_t)0xFFFFFE00);
}



// 在系统复位后, 系统时钟使用默认的内部高速时钟(HSI)
// 需要配置 RCC, 设置最终的系统时钟源
HAL_StatusTypeDef sys_clock_init(uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq) {

    HAL_StatusTypeDef ret;

    // 使能 PWR 时钟
    __HAL_RCC_PWR_CLK_ENABLE();
    // 配置电压调节器范围
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // 初始化时钟源, 并设置锁相环的分频系数
    RCC_OscInitTypeDef rcc_osc_init = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_HSE_ON,
        .LSEState = RCC_LSE_OFF,
        .HSIState = RCC_HSI_OFF,
        .HSICalibrationValue = 0x00,
        .LSIState = RCC_LSI_OFF,
        // 锁相环(Phase-Locked Loop)
        // 从一个低频参考时钟生成一个高频时钟信号, 从而提高系统的运行速度
        // F{VCO} = F{HSE} / PLLM * PLLN
        // F{SYSCLK} = F{VCO} / PLLP            // 系统时钟频率
        // F{USB_OTG_FS} = F{VCO} / PLLQ
        .PLL = {
            .PLLState = RCC_PLL_ON,
            .PLLSource = RCC_PLLSOURCE_HSE,
            .PLLM = pllm,   // 分频系数 m
            .PLLN = plln,   // 倍频系数 n
            .PLLP = pllp,   // 主分频系数 p
            .PLLQ = pllq,   // USB OTG FS, SDIO 和 RNG 时钟分频系数 q
        }
    };
    ret = HAL_RCC_OscConfig(&rcc_osc_init);
    if (ret != HAL_OK) {
        return ret;
    }

    // 过驱动模式(OverDrive Mode) 是一种特殊的电源模式
    // 允许微控制器在更高的频率下运行, 从而提高系统性能。
    // 启用过驱动模式后, 电压调节器会提供更高的电压, 以支持更高的运行频率。
    // ret = HAL_PWREx_EnableOverDrive();
    // if (ret != HAL_OK) {
    //     return ret;
    // }

    // 配置系统时钟
    RCC_ClkInitTypeDef rcc_clk_init = {
        // 设置需要配置的时钟类型
        // 系统时钟
        // 高速总线时钟(HCLK), 用于驱动内核/存储器和 AHB 总线外设
        // 低速外设总线时钟(PCLK1/APB1), 用于驱动 APB1 总线外设
        // 高速外设总线时钟(PCLK2/AHB2), 用于驱动 APB2 总线外设
        .ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2),
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV4,
        .APB2CLKDivider = RCC_HCLK_DIV2,
    };
    // 高频率运行时闪存存储器的访问速度可能无法跟上处理器的时钟速度, 因此需要插入等待周期以确保数据读取的正确性
    // 设置 FLASH 延时周期为 5WS, 即读取 FLASH 需要 6 周期
    ret = HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_5);
    if (ret != HAL_OK) {
        return ret;
    }

    sys_nvic_set_vector_table(FLASH_BASE, 0x0);
    return HAL_OK;
}