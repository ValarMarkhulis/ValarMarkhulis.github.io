/*
 * spi.c
 *
 * Created: 6/10/2017 13:54:52
 *  Author: Christian
 */ 
/*
 * SPI_master.c
 */ 

#include <avr/io.h>
#include "Header.h"

//PORTE
#define DDRx DDRB
#define SS   0
#define SCK  1
#define MOSI 2
#define MISO 3

/*
unsigned char addrTest = 0x02;
unsigned char dataTest = 0x55;
*/

unsigned char recive[10];

unsigned char SPI_MasterReceive(void){
	while(!(SPSR &(1<<SPIF)));
	return SPDR;
}

void SPI_MasterInit(){
	
	DDRB |=(1<<DDB2)|(1<<DDB1)|(1<<DDB0); // Sætter data direction registeret til output på pin 51, 52 og 53 (SPI benene).
	SPCR |=(1<<SPE)|(1<<MSTR)|(1<<CPOL);  //side 202 clock polarity: Cpol = 1, SCK er høj når idle. Leading Edge er på "falling"
	SPCR |=(1<<SPR1); SPSR |=(1<<SPI2X); //500kHz
	//SPCR |=(1<<CPHA);
	PORTB |=(1<<PB0);
}
//SCK	Grøn	Pin 52 | B2
//MISO	Gul		Pin 50 | B0
//MOSI	Hvid	Pin 51 | B1
//SS	Rød		Pin 53 | B3


void SPI_MasterTxRx(unsigned char data) {
	PORTB &=~(1<<PB0);
	//put_char(data);
	SPDR = data;
	
	while (!(SPSR &(1<<SPIF)));
		data = SPDR;
	PORTB |=(1<<PB0);
	//return data;
	
	//PORTB |=(1<<PB0);
}

unsigned char SPI_send(unsigned char addr, unsigned char data){ 
	unsigned char syncbyte = 69;
	unsigned char checkbyte = addr ^ data;
	
	/*put_char(syncbyte);
	put_char(addr);
	put_char(data);
	put_char(checkbyte);
	//put_char(0x4B);*/
	//put_char(0);
	//put_char(0);
	//SPCR |=(1<<SPIE);
	SPI_MasterTxRx(syncbyte);
	SPI_MasterTxRx(addr);
	SPI_MasterTxRx(data);
	SPI_MasterTxRx(checkbyte);
	/*recive[4] = SPI_MasterTxRx(0x33);//Dummy byte
	recive[5] = SPI_MasterTxRx(0x33);
	recive[6] = SPI_MasterTxRx(0x33);
	recive[7] = SPI_MasterTxRx(0x33);
	recive[8] = SPI_MasterTxRx(0x33);
	/ *recive[4] = SPI_MasterTxRx(0x33);
	recive[5] = SPI_MasterTxRx(0x33);
	recive[6] = SPI_MasterTxRx(0x33);
	recive[7] = SPI_MasterTxRx(0x33);* /
	//SPI_MasterTxRx(0x00);//Dummy byte
	//SPI_MasterTxRx(0x00);//Dummy byte
	//SPI_MasterTxRx(0x00);//Dummy byte
	//SPCR &=~(1<<SPIE);
	put_char(recive[0]);
	put_char(recive[1]);
	put_char(recive[2]);
	put_char(recive[3]);
	put_char(recive[4]);
	put_char(recive[5]);
	put_char(recive[6]);
	put_char(recive[7]);
	put_char(recive[8]);
	/ *put_char(recive[5]);
	put_char(recive[6]);
	put_char(recive[7]);* /
	//put_char(addr);
	if((data&0b11111001)==11111001 && addr==((data>>1)&0b00000011)){
		return 1;
	}
	else{
		return 0;
	}*/
	return 0;
}

/*


ISR(SPI_STC_vect){
	static int i=0;
	
	recive[i] = SPDR;
	put_char(recive[i]);
	if(i==7){
		i=0;
	}
	
	i++;	
}*/