#ifdef __cplusplus
extern "C"{
#endif
#ifndef __nRF24L01_MNEMONICS_H__
#define __nRF24L01_MNEMONICS_H__

#define MCU_TYPE_STM32				1
#define MCU_TYPE_AVR				2

#define R_REGISTER_CMD(addr)		(0x00 | addr)
#define W_REGISTER_CMD(addr)		(0x20 | addr)
#define R_RX_PAYLOAD_CMD			0x61
#define W_TX_PAYLOAD_CMD			0xA0
#define FLUSH_TX_CMD				0xE1
#define FLUSH_RX_CMD				0xE2
#define REUSE_TX_PL_CMD				0xE3
#define R_RX_PL_WID_CMD(pipe)		(0x60 | pipe)
#define W_ACK_PAYLOAD_CMD			0xA8
#define W_TX_PAYLOAD_NO_ACK_CMD		0xB0
#define NOP_CMD  					0xFF

#define CONFIG_REG_ADDR				0x00
#define EN_AA_REG_ADDR				0x01
#define EN_RXADDR_REG_ADDR			0x02
#define SETUP_AW_REG_ADDR			0x03
#define SETUP_RETR_REG_ADDR			0x04
#define RF_CH_REG_ADDR				0x05
#define RF_SETUP_REG_ADDR			0x06
#define STATUS_REG_ADDR				0x07
#define OBSERVE_TX_REG_ADDR			0x08
#define RPD_REG_ADDR				0x09
#define RX_ADDR_P0_REG_ADDR			0x0A
#define RX_ADDR_P1_REG_ADDR			0x0B
#define RX_ADDR_P2_REG_ADDR			0x0C
#define RX_ADDR_P3_REG_ADDR			0x0D
#define RX_ADDR_P4_REG_ADDR			0x0E
#define RX_ADDR_P5_REG_ADDR			0x0F
#define TX_ADDR_REG_ADDR			0x10
#define RX_PW_P0_REG_ADDR			0x11
#define RX_PW_P1_REG_ADDR			0x12
#define RX_PW_P2_REG_ADDR			0x13
#define RX_PW_P3_REG_ADDR			0x14
#define RX_PW_P4_REG_ADDR			0x15
#define RX_PW_P5_REG_ADDR			0x16
#define FIFO_STATUS_REG_ADDR		0x17
#define ACK_PLD_REG_ADDR			NULL
#define TX_PLD_REG_ADDR				NULL
#define RX_PLD_REG_ADDR				NULL
#define DYNPD_REG_ADDR				0x1C
#define FEATURE_REG_ADDR			0x1D



#define CONFIG_REG_DEFAULT				0x08
#define EN_AA_REG_DEFAULT				0x3F
#define EN_RXADDR_REG_DEFAULT			0x03
#define SETUP_AW_REG_DEFAULT			0x03
#define SETUP_RETR_REG_DEFAULT			0x03
#define RF_CH_REG_DEFAULT				0x02
#define RF_SETUP_REG_DEFAULT			0x0E
#define STATUS_REG_DEFAULT				0x0E
#define OBSERVE_TX_REG_DEFAULT			0x00
#define RPD_REG_DEFAULT					0x00
#define RX_ADDR_P0_REG_DEFAULT			0xE7E7E7E7E7
#define RX_ADDR_P1_REG_DEFAULT			0xC2C2C2C2C2
#define RX_ADDR_P2_REG_DEFAULT			0xC3
#define RX_ADDR_P3_REG_DEFAULT			0xC4
#define RX_ADDR_P4_REG_DEFAULT			0xC5
#define RX_ADDR_P5_REG_DEFAULT			0xC6
#define TX_ADDR_REG_DEFAULT				0xE7E7E7E7E7
#define RX_PW_P0_REG_DEFAULT			0x00
#define RX_PW_P1_REG_DEFAULT			0x00
#define RX_PW_P2_REG_DEFAULT			0x00
#define RX_PW_P3_REG_DEFAULT			0x00
#define RX_PW_P4_REG_DEFAULT			0x00
#define RX_PW_P5_REG_DEFAULT			0x00
#define FIFO_STATUS_REG_DEFAULT			0x01
#define ACK_PLD_REG_DEFAULT				NULL
#define TX_PLD_REG_DEFAULT				NULL
#define RX_PLD_REG_DEFAULT				NULL
#define DYNPD_REG_DEFAULT				0x00
#define FEATURE_REG_DEFAULT				0x00

typedef enum{
	PRIM_PTX,
	PRIM_PRX
}prim_rx_t;
typedef enum{
	POWER_DOWN,
	POWER_UP
}pwr_up_t;
typedef enum{
	_1_BYTE,
	_2_BYTE,
}crco_t;

typedef enum{
	_3_BYTE = 0x01,
	_4_BYTE,
	_5_BYTE
}aw_t;
typedef enum{
	_250uS,
	_500uS,
	_750uS,
	_1000uS,
	_1250uS,
	_1500uS,
	_1750uS,
	_2000uS,
	_2250uS,
	_2500uS,
	_2750uS,
	_3000uS,
	_3250uS,
	_3500uS,
	_3750uS,
	_4000uS
}ard_t;
typedef enum{
	DSBL_ReTransmisson,
	_1_ReTransmission,
	_2_ReTransmission,
	_3_ReTransmission,
	_4_ReTransmission,
	_5_ReTransmission,
	_6_ReTransmission,
	_7_ReTransmission,
	_8_ReTransmission,
	_9_ReTransmission,
	_10_ReTransmission,
	_11_ReTransmission,
	_12_ReTransmission,
	_13_ReTransmission,
	_14_ReTransmission,
	_15_ReTransmission
}arc_t;

typedef enum{
	_1_Mbps,
	_2_Mbps,
	_250_Kbps
}rf_data_rate_t;
typedef enum{
	_18_dBm,
	_12_dBm,
	_6_dBm,
	_0_dBm
}rf_pwr_t;

typedef enum{
	PIPE_0,
	PIPE_1,
	PIPE_2,
	PIPE_3,
	PIPE_4,
	PIPE_5
}pipe_t;

typedef enum{
	PRIM_RX,
	PWR_UP,
	CRCO,
	EN_CRC,
	MASK_MAX_RT,
	MASK_TX_DS,
	MASK_RX_DR
}CONFIG_Bit_t;

typedef enum{
	ENAA_P0,
	ENAA_P1,
	ENAA_P2,
	ENAA_P3,
	ENAA_P4,
	ENAA_P5
}EN_AA_Bit_t;

typedef enum{
	ERX_P0,
	ERX_P1,
	ERX_P2,
	ERX_P3,
	ERX_P4,
	ERX_P5
}EN_RXADDR_Bit_t;

typedef enum{
	ARC = 0x00,
	ARD = 0x04
}SETUP_RETR_Bit_t;

typedef enum{
	RF_PWR = 0x01,
	RF_DR_HIGH = 0x03,
	PLL_LOCK = 0x04,
	RF_DR_LOW = 0x05,
	CONT_WAVE = 0x07
}RF_SETUP_Bit_t;

typedef enum{
	TX_STATUS_FULL = 0x00,
	RX_P_NO = 0x01,
	MAX_RT = 0x04,
	TX_DS = 0x05,
	RX_DR = 0x06
}STATUS_Bit_t;

typedef enum{
	ARC_CNT = 0x00,
	PLOS_CNT = 0x04
}OBSERVE_TX_Bit_t;

typedef enum{
	RX_EMPTY = 0x00,
	RX_FULL = 0x01,
	TX_EMPTY = 0x04,
	TX_FIFO_FULL = 0x05,
	TX_RESUE = 0x06
}FIFO_STATUS_Bit_t;

typedef enum{
	DPL_P0,
	DPL_P1,
	DPL_P2,
	DPL_P3,
	DPL_P4,
	DPL_P5
}DYNPD_Bit_t;

typedef enum{
	EN_DYN_ACK,
	EN_ACK_PAY,
	EN_DPL
}FEATURE_Bit_t;



#define Tpwrdwn2stndby		1500//ms
#define Tstndby2RxTxModes	1//ms

#endif
#ifdef __cplusplus
}
#endif
