#ifdef __cplusplus
extern "C"{
#endif
#ifndef __nRF24L01_H__
#define __nRF24L01_H__
#include "nRF24L01_config.h"
#include "nRF24L01_mnemonics.h"

typedef struct nRF24L01Set{
	rf_data_rate_t rf_data_rate;
	uint8_t rf_channel;
	rf_pwr_t rf_power;
	pipe_t pipe;
	prim_rx_t current_primX;
	uint32_t pipe_addr;
}nRF24L01Set_t, *nRF24L01Set_ptr_t;

typedef enum{
	TX_DS_FLAG_SET = (1 << TX_DS),
	MAX_RT_FLAG_SET = (1 << MAX_RT),
	RX_DR_FLAG_SET = (1 << RX_DR)
}nRF24L01_STATUS_t;

#if(USED_MCU_TYPE == MCU_TYPE_STM32)
void nRF24L01_init(SPI_HandleTypeDef *spi_ptr, nRF24L01Set_ptr_t nRF24Set_ptr);
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
void nRF24L01_init(nRF24L01Set_ptr_t nRF24Set_ptr);
#endif
void nRF24L01_IRQ_allDisable(void);
void nRF24L01_Pipe_allDisable(void);
void nRF24L01_Pipe_Enable(pipe_t pipe);
void nRF24L01_Pipe_Disable(pipe_t pipe);
void nRF24L01_setAddrWidth(aw_t aw);
void nRF24L01_setAutoRetr(arc_t arc, ard_t ard);
void nRF24L01_setRFChannel(uint8_t channel);
void nRF24L01_setDataRate(rf_data_rate_t data_rate);
void nRF24L01_setRFPower(rf_pwr_t power);
void nRF24L01_PowerUp(void);
void nRF24L01_PowerDown(void);
void nRF24L01_setPipeAddr(pipe_t pipe, uint32_t addr);
void nRF24L01_DynamicPayload_Enable(pipe_t pipe);
void nRF24L01_DynamicPayload_Disable(pipe_t pipe);
void nRF24L01_setPRIMx(prim_rx_t primx);
nRF24L01_STATUS_t nRF24L01_Transmit(uint8_t data[], uint8_t data_size);
void nRF24L01_listen(void);
uint8_t nRF24L01_available(void);
uint8_t nRF24L01_getPayloadWidth(void);
void nRF24L01_Receive(uint8_t data[], uint8_t data_size);



#endif
#ifdef __cplusplus
}
#endif
