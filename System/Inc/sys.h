#ifndef __SYS_H
#define __SYS_H

#include "stm32f4xx_hal.h"

void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset);
HAL_StatusTypeDef sys_clock_init(uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq);


#endif