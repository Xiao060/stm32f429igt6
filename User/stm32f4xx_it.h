/**
  ******************************************************************************
  * @file    Templates/Inc/stm32f4xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

// 定义中断处理函数

// 复位中断处理程序
// Reset_Handler 在 startup_stm32f429xx.s 中实现

// 非屏蔽中断
void NMI_Handler(void);
// 硬件故障中断
void HardFault_Handler(void);
// 内存管理中断
void MemManage_Handler(void);
// 总线故障中断
void BusFault_Handler(void);
// 使用错误中断
void UsageFault_Handler(void);
// 服务调用中断
void SVC_Handler(void);
// 调试监控中断
void DebugMon_Handler(void);
// 未定义指令中断
void PendSV_Handler(void);
// 系统滴答中断
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
