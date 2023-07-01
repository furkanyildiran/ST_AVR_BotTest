#include "usart.h"
#define USART_UDRE_INT_ENBL (UCSR1B|=(1<<UDRIE1))
#define USART_UDRE_INT_DSBL (UCSR1B&=~(1<<UDRIE1))

#define UBRR_FUNC(baudrate) (F_CPU/(16*baudrate))-1
#define UBRR2x_FUNC(baudrate) (F_CPU/(8*baudrate))-1

uint8_t tx_ring_buff[TX_BUFF_SIZE];
volatile uint8_t tx_head = 0, tx_tail = 0;
volatile uint8_t parity_error=0;
volatile uint8_t rx_data;
volatile uint8_t rx_available_flag = 0;

ISR (USART1_UDRE_vect){
	UDR1 = tx_ring_buff[tx_tail++];
	if(tx_head==tx_tail){
		tx_head=0;
		tx_tail=0;
		USART_UDRE_INT_DSBL;
	}
}

ISR (USART1_RX_vect){
	rx_available_flag = 1;
	rx_data = UDR1;
}

void USART_init(uint32_t baudrate, uint8_t char_size){
	uint16_t ubrr_val = (uint16_t)UBRR_FUNC(baudrate);
	if(baudrate>9600){
		ubrr_val = (uint16_t)UBRR2x_FUNC(baudrate);
		UCSR1A |= (1 << U2X0);
	}
	UBRR1L = ubrr_val;
	UBRR1H = (ubrr_val>>8);
	UCSR1B = (1<<RXEN0) | (1<<TXEN0) | ((char_size>>2)<<UCSZ02) | (1 << RXCIE0);
	UCSR1C = ((char_size&1)<<UCSZ00) | (((char_size&2)>>1)<<UCSZ01) | (1 << USBS0);
	UDR1=0;
}

void USART_transmit(uint8_t data[],uint8_t size){
	for (uint8_t i=0; i < size; i++){
		tx_ring_buff[tx_head++] = data[i];
	}
	USART_UDRE_INT_ENBL;
	while(tx_head!=tx_tail);
}

uint8_t USART_receive(void){
	rx_available_flag=0;
	return rx_data;
}

uint8_t USART_available(void){
	return rx_available_flag;
}

void USART_parity_enbl(uint8_t mode){
	UCSR1C |= ((mode>>1)<<UPM01) | ((mode&0b01)<<UPM00);
}

uint8_t USART_parity_err_check(void){
	return (parity_error&(1<<UPE0));
}

void USART_parity_err_clear(void){
	UCSR1A &= ~(1<<UPE0);
}