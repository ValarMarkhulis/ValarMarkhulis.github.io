/*
 * uart.c
 *
 * Created: 6/10/2017 13:52:38
 *  Author: Christian
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Header.h"

unsigned int record_len = 500;


void init_uart0(int baud){
	UBRR0H =(unsigned char)baud>>8;
	UBRR0L =(unsigned char)baud;
	UCSR0A =(1<<U2X0);
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
	UCSR1B |=(1<<TXCIE0);//Enable transmit interrupt 
}
void init_uart1(int baud){
	UBRR1H =(unsigned char)baud>>8;
	UBRR1L =(unsigned char)baud;
	UCSR1A =(1<<U2X1);
	UCSR1B|=(1<<RXEN1)|(1<<TXEN1);
	UCSR1C|=(1<<UCSZ10)|(1<<UCSZ11);
	UCSR1B |=(1<<RXCIE1);//enable receive interrupt
	UCSR1B |=(1<<TXCIE1);//enable transmit interrupt
}

//Disable og enable af TX complete for UART 1
void EN_intr_TX(){
	UCSR1B |=(1<<TXCIE1);
}
void DIS_intr_TX(){
	UCSR1B &=~(1<<TXCIE1);
}

char isTXintr_enabled(){
	if((UCSR1B & (1<<TXCIE1))==0){
		return 0;
	}
	
	return 1;
}
	

void put_char(char data){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=data;
}

void put_char1(char data){
	while(!(UCSR1A&(1<<UDRE1)));
	UDR1=data;
}

//Sender data til UART 0 & 1 via polling
void send_data0(){
	unsigned char data_out, checksum = 0;
	int i;
	checksum ^= 0x55^0xAA^(((record_len+7)&0xFF00)>>8)^((record_len+7)&0x00FF)^0x02;		
	put_char(0x55);
	put_char(0xAA);
	put_char(((record_len+7)&0xFF00)>>8);
	put_char((record_len+7)&0x00FF);
	put_char(0x02);
	for(i=0;i<record_len;i++){
		data_out = removeData();
		checksum ^= data_out;
		put_char(data_out);
	}
	checksum ^=0x00;
	put_char(0x00);
	put_char(checksum);
}
void send_data1(){
	unsigned char data_out, checksum = 0;
	int i;
	checksum ^= 0x55^0xAA^(((record_len+7)&0xFF00)>>8)^((record_len+7)&0x00FF)^0x02;
	put_char1(0x55);
	put_char1(0xAA);
	put_char1(((record_len+7)&0xFF00)>>8);
	put_char1((record_len+7)&0x00FF);
	put_char1(0x02);
	for(i=0;i<record_len;i++){
		data_out = removeData();
		checksum ^= data_out;
		put_char1(data_out);
	}
	checksum ^=0x00;
	put_char1(0x00);
	put_char1(checksum);
}
//Sender data til generator menuen via polling
void send_generator(char button, char value, char *ind, char *sha, char *amp, char *fre ){
	static unsigned char indicator=0, chksum=0;
	static unsigned char data[] = {0x55,0xAA,0x00,0x0B,0x01,0x00,0x00,0x00,0x00,0x00};
	unsigned char i = 0;
	
	switch(button){
		
		case 0 :
		data[indicator+6]=value;
		button = 5;
		break;
		case 1 :
		indicator++;
		if(indicator == 3)
		{
			indicator = 0;
		}
		data[5] = indicator;
		button = 5;
		break;
		case 2 :
		data[5] = 0; // indicator
		data[6] = 0; // shape
		data[7] = 0; // amplitude
		data[8] = 0; // Frequency
		button = 5;
		break;
	}
	
	for(i=0;i<10;i++){
		chksum ^= data[i];
		put_char1(data[i]);
		
	}
	put_char1(chksum);
	*ind = data[5];// Returnerer indkator værdien
	*sha = data[6];// Returnerer shape værdien
	*amp = data[7];// Returnerer amplitude værdien
	*fre = data[8];// Returnerer frekvens værdi
	chksum = 0;
}
