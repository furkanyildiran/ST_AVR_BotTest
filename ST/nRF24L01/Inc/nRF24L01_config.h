#ifdef __cplusplus
extern "C"{
#endif
#ifndef __nRF24L01_CONFIG_H__
#define __nRF24L01_CONFIG_H__
#include "stm32f0xx.h"
#include "nRF24L01_mnemonics.h"

#define USED_MCU_TYPE				MCU_TYPE_STM32
#if(USED_MCU_TYPE == MCU_TYPE_STM32)
#include "stm32f0xx.h"
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
#include <avr/io.h>
#define nRF24L01_CE_DDR				DDRL
#define nRF24L01_CSN_DDR			DDRL
#endif
#define nRF24L01_CE_PORT 			GPIOB
#define nRF24L01_CE_PIN 			GPIO_PIN_11
#define nRF24L01_CSN_PORT			GPIOB
#define nRF24L01_CSN_PIN 			GPIO_PIN_9

#define nRF24L01_DEFAULT_AW			_3_BYTE
#define nRF24L01_DEFAULT_ARD		_1000uS
#define nRF24L01_DEFAULT_ARC		_15_ReTransmission

#endif
#ifdef __cplusplus
}
#endif
