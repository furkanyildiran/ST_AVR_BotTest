#ifdef __cplusplus
extern "C"{
#endif
#ifndef __nRF24L01_CONFIG_H__
#define __nRF24L01_CONFIG_H__
#include "nRF24L01_mnemonics.h"

#define USED_MCU_TYPE				MCU_TYPE_AVR
#if(USED_MCU_TYPE == MCU_TYPE_STM32)
#include "stm32f0xx.h"
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "spi_custom.h"
#define nRF24L01_CE_DDR				DDRG
#define nRF24L01_CSN_DDR			DDRG
#endif
#define nRF24L01_CE_PORT 			PORTG
#define nRF24L01_CE_PIN 			0
#define nRF24L01_CSN_PORT			PORTG
#define nRF24L01_CSN_PIN 			1

#define nRF24L01_DEFAULT_AW			_3_BYTE
#define nRF24L01_DEFAULT_ARD		_1000uS
#define nRF24L01_DEFAULT_ARC		_15_ReTransmission

#endif
#ifdef __cplusplus
}
#endif
