#ifndef VFD_H
#define VFD_H

#include "stm32l0xx_hal.h"

#include "string.h"
#include "stdint.h"
// Your declarations go here
#define VFD_CSL_Set0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define VFD_CSL_Set1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define VFD_RST_Set0 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define VFD_RST_Set1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define VFD_ENA_Set0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define VFD_ENA_Set1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)

#define din     GPIO_PIN_7
#define din_0   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define din_1   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define clk     GPIO_PIN_5
#define clk_0   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define clk_1   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define cs      GPIO_PIN_6  //片选
#define cs_0    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
#define cs_1    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
#define reset   GPIO_PIN_0
#define reset_0 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
#define reset_1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#define EN      GPIO_Pin_15  //使能
#define EN_0    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
#define EN_1    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

void write_6302(unsigned char w_data);
void VFD_init(void);
void S1201_show(void);
void S1201_WriteOneChar(unsigned char x, unsigned char chr);
void S1201_WriteStr(unsigned char x, char *str);
void VFD_test_out(void) ;



#endif // VFD_H