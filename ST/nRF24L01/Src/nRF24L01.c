#include "nRF24L01.h"

#include <stdio.h>
//extern UART_HandleTypeDef huart1;

#if(USED_MCU_TYPE == MCU_TYPE_STM32)
#define CSN_LOW()		(HAL_GPIO_WritePin(nRF24L01_CSN_PORT, nRF24L01_CSN_PIN, GPIO_PIN_RESET))
#define CSN_HIGH()		(HAL_GPIO_WritePin(nRF24L01_CSN_PORT, nRF24L01_CSN_PIN, GPIO_PIN_SET))
#define CE_LOW()		(HAL_GPIO_WritePin(nRF24L01_CE_PORT, nRF24L01_CE_PIN, GPIO_PIN_RESET))
#define CE_HIGH()		(HAL_GPIO_WritePin(nRF24L01_CE_PORT, nRF24L01_CE_PIN, GPIO_PIN_SET))
#define CE_PULSE()		do{CE_HIGH(); HAL_Delay(1); CE_LOW();}while(0)
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
#define CSN_LOW()		(nRF24L01_CSN_PORT &= ~(1 << nRF24L01_CSN_PIN))
#define CSN_HIGH()		(nRF24L01_CSN_PORT |=  (1 << nRF24L01_CSN_PIN))
#define CE_LOW()		(nRF24L01_CE_PORT  &= ~(1 << nRF24L01_CE_PIN))
#define CE_HIGH()		(nRF24L01_CE_PORT  |=  (1 << nRF24L01_CE_PIN))
#define CE_PULSE()		do{CE_HIGH(); _delay_ms(1); CE_LOW();}while(0)
#endif

#define TX_STATUS_FLAGS_CONTROL()			((getStatusReg() & (TX_DS_FLAG_SET)) ? TX_DS_FLAG_SET : MAX_RT_FLAG_SET)
#define RX_STATUS_FLAG_CONTROL()			(getStatusReg() & RX_DR_FLAG_SET)

#define RADIO_MODE_CONTROL()				(radio_mode)
#define RADIO_IN_PTX_MODE()					(radio_mode = RADIO_IS_IN_PTX)
#define RADIO_IN_PRX_MODE()					(radio_mode = RADIO_IS_IN_PRX)

static SPI_HandleTypeDef *nRF_SPI_ptr;
static nRF24L01Set_ptr_t nRF_Set_ptr;
typedef enum{
	RADIO_IS_IN_PRX,
	RADIO_IS_IN_PTX
}radio_mode_t;
static radio_mode_t radio_mode;

static void reset_registers(void);
static void flush_tx(void);
static void flush_rx(void);

static void write_reg_byte(uint8_t addr, uint8_t data);
static uint8_t read_reg_byte(uint8_t addr);
static void write_data(uint8_t addr, uint8_t data[], uint8_t data_size);
static void read_data(uint8_t addr, uint8_t data[], uint8_t data_size);
static uint8_t getStatusReg(void);
static void clearStatusReg(void);

extern uint8_t status1,status2,status3;

char debug[30]={"\0"};
void debugf(uint8_t size){
//	HAL_UART_Transmit(&huart1, (uint8_t*)debug, size, 1000);
	for(uint8_t i= 0; i < 30; i++) debug[i]='\0';
}

extern uint8_t dbg_val;

#if(USED_MCU_TYPE == MCU_TYPE_STM32)
void nRF24L01_init(SPI_HandleTypeDef *spi_ptr, nRF24L01Set_ptr_t nRF24Set_ptr){
	CE_LOW();
	CSN_HIGH();
	nRF_SPI_ptr = spi_ptr;
	nRF_Set_ptr = nRF24Set_ptr;
	RADIO_IN_PTX_MODE();
	reset_registers();
	nRF24L01_Pipe_allDisable();
	nRF24L01_IRQ_allDisable();
	nRF24L01_setAutoRetr(nRF24L01_DEFAULT_ARC, nRF24L01_DEFAULT_ARD);
	nRF24L01_setRFChannel(nRF24Set_ptr->rf_channel);
	nRF24L01_setDataRate(nRF24Set_ptr->rf_data_rate);
	nRF24L01_setRFPower(nRF24Set_ptr->rf_power);
	nRF24L01_Pipe_Enable(nRF24Set_ptr->pipe);
	nRF24L01_setAddrWidth(nRF24L01_DEFAULT_AW);
	nRF24L01_setPipeAddr(nRF24Set_ptr->pipe, nRF24Set_ptr->pipe_addr);
	nRF24L01_DynamicPayload_Enable(nRF24Set_ptr->pipe);
	nRF24L01_PowerUp();
	clearStatusReg();
}
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
void nRF24L01_init(nRF24L01Set_ptr_t nRF24Set_ptr){
	nRF24L01_CSN_DDR |= (1 << nRF24L01_CSN_PIN);
	nRF24L01_CE_DDR |= (1 << nRF24L01_CE_PIN);
	CE_LOW();
	CSN_HIGH();
	nRF_Set_ptr = nRF24Set_ptr;
	RADIO_IN_PTX_MODE();
	reset_registers();
	nRF24L01_Pipe_allDisable();
	nRF24L01_IRQ_allDisable();
	nRF24L01_setAutoRetr(nRF24L01_DEFAULT_ARC, nRF24L01_DEFAULT_ARD);
	nRF24L01_setRFChannel(nRF24Set_ptr->rf_channel);
	nRF24L01_setDataRate(nRF24Set_ptr->rf_data_rate);
	nRF24L01_setRFPower(nRF24Set_ptr->rf_power);
	nRF24L01_Pipe_Enable(nRF24Set_ptr->pipe);
	nRF24L01_setAddrWidth(nRF24L01_DEFAULT_AW);
	nRF24L01_setPipeAddr(nRF24Set_ptr->pipe, nRF24Set_ptr->pipe_addr);
	nRF24L01_DynamicPayload_Enable(nRF24Set_ptr->pipe);
	nRF24L01_PowerUp();
	clearStatusReg();
}
#endif

void nRF24L01_IRQ_allDisable(void){
	uint8_t val = read_reg_byte(CONFIG_REG_ADDR);
	val |= (1 << MASK_RX_DR) | (1 << MASK_TX_DS) | (1 << MASK_MAX_RT);
	write_reg_byte(CONFIG_REG_ADDR, val);
}

void nRF24L01_Pipe_allDisable(void){
	write_reg_byte(EN_AA_REG_ADDR, 0x00);
	write_reg_byte(EN_RXADDR_REG_ADDR, 0x00);
}

void nRF24L01_Pipe_Enable(pipe_t pipe){
	write_reg_byte(EN_AA_REG_ADDR, (1 << pipe));
	write_reg_byte(EN_RXADDR_REG_ADDR, (1 << pipe));
}

void nRF24L01_Pipe_Disable(pipe_t pipe){
	uint8_t temp = read_reg_byte(EN_AA_REG_ADDR);
	temp &= ~(1 << pipe);
	write_reg_byte(EN_AA_REG_ADDR, temp);

	temp = read_reg_byte(EN_RXADDR_REG_ADDR);
	temp &= ~(1 << pipe);
	write_reg_byte(EN_RXADDR_REG_ADDR, temp);
}

void nRF24L01_setAddrWidth(aw_t aw){
	write_reg_byte(SETUP_AW_REG_ADDR, aw);
}

void nRF24L01_setAutoRetr(arc_t arc, ard_t ard){
	uint8_t val = (ard << ARD) | (arc << ARC);
	write_reg_byte(SETUP_RETR_REG_ADDR, val);
}

void nRF24L01_setRFChannel(uint8_t channel){
	write_reg_byte(RF_CH_REG_ADDR, channel);
}

void nRF24L01_setDataRate(rf_data_rate_t data_rate){
	uint8_t val=read_reg_byte(RF_SETUP_REG_ADDR);
	val &= 0x96;
	val |= ((((data_rate>>1) & 0x1) << RF_DR_LOW) | ((data_rate & 0x1) << RF_DR_HIGH));
	write_reg_byte(RF_SETUP_REG_ADDR, val);
}

void nRF24L01_setRFPower(rf_pwr_t power){
	uint8_t val = read_reg_byte(RF_SETUP_REG_ADDR);
	val &= 0x96;
	val |= (((power>>1) & 0x1) << 2) | ((power & 0x1) << 1);
	write_reg_byte(RF_SETUP_REG_ADDR, val);
}

void nRF24L01_PowerUp(void){
	uint8_t val = read_reg_byte(CONFIG_REG_ADDR);
	val |= (1 << PWR_UP);
	write_reg_byte(CONFIG_REG_ADDR, val);
}
void nRF24L01_PowerDown(void){
	uint8_t val = read_reg_byte(CONFIG_REG_ADDR);
	val &= ~(1 << PWR_UP);
	write_reg_byte(CONFIG_REG_ADDR, val);
}

void nRF24L01_setPipeAddr(pipe_t pipe, uint32_t addr){
	if(pipe < PIPE_2){
		uint8_t addr_buff[5]={0}, addr_width = nRF24L01_DEFAULT_AW + 2;

		for(uint8_t index = 0, shifting = 0; index < addr_width; index++, shifting += 8)
			addr_buff[index] = (addr >> shifting)&0xFF;

		write_data(W_REGISTER_CMD((pipe + RX_ADDR_P0_REG_ADDR)), addr_buff, addr_width);
		write_data(W_REGISTER_CMD(TX_ADDR_REG_ADDR), addr_buff, addr_width);
		return;
	}
	write_reg_byte((pipe + RX_ADDR_P0_REG_ADDR), (uint8_t)addr);
	write_reg_byte(TX_ADDR_REG_ADDR, (uint8_t)addr);
}

void nRF24L01_DynamicPayload_Enable(pipe_t pipe){
	write_reg_byte(FEATURE_REG_ADDR, (1 << EN_DPL));
	write_reg_byte(DYNPD_REG_ADDR, (1 << pipe));
}

void nRF24L01_DynamicPayload_Disable(pipe_t pipe){
	uint8_t temp = read_reg_byte(DYNPD_REG_ADDR);
	temp &= ~(1 << pipe);
	write_reg_byte(DYNPD_REG_ADDR, temp);
	write_reg_byte(FEATURE_REG_ADDR, 0x00);
}

void nRF24L01_setPRIMx(prim_rx_t primx){
	uint8_t temp = read_reg_byte(CONFIG_REG_ADDR);
	temp &= 0xFE;
	temp |= (primx << PRIM_RX);
	write_reg_byte(CONFIG_REG_ADDR, temp);
}

nRF24L01_STATUS_t nRF24L01_Transmit(uint8_t data[], uint8_t data_size){
	CE_LOW();
	flush_rx();
	flush_tx();
	if(RADIO_MODE_CONTROL() != RADIO_IS_IN_PTX){
		RADIO_IN_PTX_MODE();
		nRF24L01_setPRIMx(PRIM_PTX);
	}
	write_data(W_TX_PAYLOAD_CMD, data, data_size);
	CE_PULSE();
	nRF24L01_STATUS_t status=0;
	do{
		status = TX_STATUS_FLAGS_CONTROL();
	}while(!(status & (TX_DS_FLAG_SET | MAX_RT_FLAG_SET)));
	flush_tx();
	flush_rx();
	clearStatusReg();
	return status;
}

void nRF24L01_listen(void){
	if(RADIO_MODE_CONTROL() != RADIO_IS_IN_PRX){
		RADIO_IN_PRX_MODE();
		nRF24L01_setPRIMx(PRIM_PRX);
		CE_HIGH();
	}
}

uint8_t nRF24L01_available(void){
	return RX_STATUS_FLAG_CONTROL();
}

uint8_t nRF24L01_getPayloadWidth(void){
	return read_reg_byte(R_RX_PL_WID_CMD(nRF_Set_ptr->pipe));
}

void nRF24L01_Receive(uint8_t data[], uint8_t data_size){
	read_data(R_RX_PAYLOAD_CMD, data, data_size);
	flush_rx();
	flush_tx();
	clearStatusReg();
}

static uint8_t getStatusReg(void){
	return read_reg_byte(STATUS_REG_ADDR);
}
static void clearStatusReg(void){
	uint8_t status = (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT);
	write_reg_byte(STATUS_REG_ADDR, status);
}

static void flush_tx(void){
	uint8_t dummy_buff=0;
	write_data(FLUSH_TX_CMD, &dummy_buff, 1);
}
static void flush_rx(void){
	uint8_t dummy_buff=0;
	write_data(FLUSH_RX_CMD, &dummy_buff, 1);
}

static void reset_registers(void){
	write_reg_byte(CONFIG_REG_ADDR, CONFIG_REG_DEFAULT);
	write_reg_byte(EN_AA_REG_ADDR, EN_AA_REG_DEFAULT);
	write_reg_byte(EN_RXADDR_REG_ADDR, EN_RXADDR_REG_DEFAULT);
	write_reg_byte(SETUP_AW_REG_ADDR, SETUP_AW_REG_DEFAULT);
	write_reg_byte(SETUP_RETR_REG_ADDR, SETUP_RETR_REG_DEFAULT);
	write_reg_byte(RF_CH_REG_ADDR, RF_CH_REG_DEFAULT);
	write_reg_byte(RF_SETUP_REG_ADDR, RF_SETUP_REG_DEFAULT);
	uint8_t default_pipe_addr[5]={0};
	for(uint8_t i = 0, shifting = 0; i < 5; i++, shifting += 8)
		default_pipe_addr[i] = (RX_ADDR_P0_REG_DEFAULT >> shifting) & 0xFF;

	write_data(W_REGISTER_CMD(RX_ADDR_P0_REG_ADDR), default_pipe_addr, 5);
	for(uint8_t i = 0, shifting = 0; i < 5; i++, shifting += 8)
		default_pipe_addr[i] = (RX_ADDR_P1_REG_DEFAULT << shifting) & 0xFF;

	write_data(W_REGISTER_CMD(RX_ADDR_P1_REG_ADDR), default_pipe_addr, 5);
	write_reg_byte(RX_ADDR_P2_REG_ADDR, RX_ADDR_P2_REG_DEFAULT);
	write_reg_byte(RX_ADDR_P3_REG_ADDR, RX_ADDR_P3_REG_DEFAULT);
	write_reg_byte(RX_ADDR_P4_REG_ADDR, RX_ADDR_P4_REG_DEFAULT);
	write_reg_byte(RX_ADDR_P5_REG_ADDR, RX_ADDR_P5_REG_DEFAULT);
	for(uint8_t i = 0, shifting = 0; i < 5; i++, shifting += 8)
		default_pipe_addr[i] = (TX_ADDR_REG_DEFAULT >> shifting) & 0xFF;

	write_data(W_REGISTER_CMD(TX_ADDR_REG_ADDR), default_pipe_addr, 5);
	write_reg_byte(RX_PW_P0_REG_ADDR, RX_PW_P0_REG_DEFAULT);
	write_reg_byte(RX_PW_P1_REG_ADDR, RX_PW_P1_REG_DEFAULT);
	write_reg_byte(RX_PW_P2_REG_ADDR, RX_PW_P2_REG_DEFAULT);
	write_reg_byte(RX_PW_P3_REG_ADDR, RX_PW_P3_REG_DEFAULT);
	write_reg_byte(RX_PW_P4_REG_ADDR, RX_PW_P4_REG_DEFAULT);
	write_reg_byte(RX_PW_P5_REG_ADDR, RX_PW_P5_REG_DEFAULT);
	write_reg_byte(DYNPD_REG_ADDR, DYNPD_REG_DEFAULT);
	write_reg_byte(FEATURE_REG_ADDR, FEATURE_REG_DEFAULT);
	flush_tx();
	flush_rx();
}

static void write_reg_byte(uint8_t addr, uint8_t data){
	uint8_t buff[2]={W_REGISTER_CMD(addr),data};
	CSN_LOW();
#if(USED_MCU_TYPE == MCU_TYPE_STM32)
	HAL_SPI_Transmit(nRF_SPI_ptr, buff, 2, 1000);
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
	for(uint8_t i = 0; i < 2; i++)
		SPI_transaction(buff[i]);
#endif
	CSN_HIGH();
}

static uint8_t read_reg_byte(uint8_t addr){
	uint8_t command = R_REGISTER_CMD(addr), dummy_status=0, ret_val = 0;
	CSN_LOW();
#if(USED_MCU_TYPE == MCU_TYPE_STM32)
	HAL_SPI_TransmitReceive(nRF_SPI_ptr, &command, &dummy_status, 1, 1000);
	HAL_SPI_Receive(nRF_SPI_ptr, &ret_val, 1, 1000);
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
	dummy_status = SPI_transaction(command);
	ret_val = SPI_transaction(NOP_CMD);
#endif
	CSN_HIGH();
	return ret_val;
}

static void write_data(uint8_t addr, uint8_t data[], uint8_t data_size){
	CSN_LOW();
#if(USED_MCU_TYPE == MCU_TYPE_STM32)
	HAL_SPI_Transmit(nRF_SPI_ptr, &addr, 1, 1000);
	HAL_SPI_Transmit(nRF_SPI_ptr, data, data_size, 1000);
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
	SPI_transaction(addr);
	for(uint8_t i = 0; i < data_size; i++)
		SPI_transaction(data[i]);
#endif
	CSN_HIGH();
}

static void read_data(uint8_t addr, uint8_t data[], uint8_t data_size){
	CSN_LOW();
	uint8_t command = addr, status = 0;
#if(USED_MCU_TYPE == MCU_TYPE_STM32)
	HAL_SPI_TransmitReceive(nRF_SPI_ptr, &command, &status, 1, 1000);
	HAL_SPI_Receive(nRF_SPI_ptr, data, data_size, 1000);
#elif(USED_MCU_TYPE == MCU_TYPE_AVR)
	status = SPI_transaction(command);
	for(uint8_t i = 0; i < data_size; i++)
		data[i] = SPI_transaction(NOP_CMD);
#endif
	CSN_HIGH();
}

