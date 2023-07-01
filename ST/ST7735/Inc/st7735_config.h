#ifdef __cplusplus
extern "C"{
#endif
#ifndef __ST7735_CONFIG_H__
#define __ST7735_CONFIG_H__
#include "stm32f0xx_hal.h"

#define CS_PORT		GPIOB
#define DC_PORT		GPIOB
#define RST_PORT	GPIOB
#define CS		 	9
#define DC		 	8
#define RST		 	7

#endif/*__ST7735_CONFIG_H__*/
#ifdef __cplusplus
}
#endif
