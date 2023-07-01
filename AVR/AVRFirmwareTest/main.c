/*
 * main.c
 *
 * Created: 1.07.2023 22:41:27
 *  Author: MONSTER
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "spi_custom.h"
#include "nRF24L01.h"
#include "pcd8544.h"
#include "stepper.h"
#include "qmc5883l.h"
#include "hcsr04.h"

nRF24L01Set_t nRF={.pipe=PIPE_0,.pipe_addr=3169,.rf_channel=108,.rf_data_rate=_1_Mbps,.rf_power=_18_dBm,.current_primX=PRIM_PRX};
int main(void){
	
	SPI_master_init(SPI_SCK_DVDR4);
	PCD_init((volatile uint8_t* const)(GPIO_PORTL_ADDR), 5 ,7, 6);
	PCD_contrast(65);
	PCD_clear_all();
	A4988_init();
	
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A=249;
	nRF24L01_init(&nRF);
	HCSR04_init();
	sei();
	while(1){
		nRF24L01_listen();
		if (nRF24L01_available()){
			uint8_t data[1];
			uint8_t pw = nRF24L01_getPayloadWidth();
			nRF24L01_Receive(data,pw);
			char pcd_text[20]={"\0"};
			PCD_clear(0,80,LINE_0);	
			sprintf(pcd_text,"Received Data:%u",data[0]);
			PCD_text(pcd_text,0,LINE_0);
			uint8_t distance = HCSR04_measure();
			sprintf(pcd_text,"Distance:%u",distance);
			PCD_text(pcd_text,0,LINE_1);
			switch(data[0]){
				case 1:
				A4988_left(360);
				break;
				case 2:
				A4988_right(360);
				break;
				case 3:
				A4988_backward(200);
				break;
				case 4:
				A4988_forward(200);
				break;
				case 5:
				A4988_stop();
				break;
				default:break;
			}
		}
	}
	return 0;
}