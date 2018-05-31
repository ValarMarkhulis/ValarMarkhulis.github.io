/*
 * 3ugers.c
 *
 * Created: 08-06-2017 10:48:23
 * Author : Nicholas Pihl
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "Header.h"


//Definitioner
enum {Default,Generator, Oscilloskop, Bodeplot};
enum {Enter,Select,RunStop,Reset};
enum {ZERO16, LRC8, CRC16};
char checksum = ZERO16;

#define True 1
#define False 0

//UART STUFF SERVICE ROUTINES
volatile unsigned char UART1=0, Generator_data[5];
volatile int package_len;
unsigned char buff1[50], msg_type = Oscilloskop;
volatile unsigned char amp[256];

//FUNKTIONER
volatile int Checksum(int len, char type, unsigned char *data);
char amplitude();
void bodeplot();


void setup (){
	sei();
	init_ADC_interrupt_AREF(0);
	init_timer1(1000);
	init_uart0(16); //baud rate 115.2K | Når værdien er 16(giver ca 2% fejl)
	init_uart1(16);
	SPI_MasterInit();
	DIS_intr_TX();//Disable TXInt
	UDR1 = 0;//Sender til uart for at enable interrupt
}

int main(void)
{
    setup();
	unsigned char Lab_receive[50], type=0, button=5, priority=2, menu=0, state = 0;
	unsigned int i=0, o=0, k=0;
	long min_record, sample = 1000;
	
	
    while (1) 
    {
	/////////////////////////
	//     UART RECIVE     //
	/////////////////////////
		if(UART1==1 && isTXintr_enabled()==False){ //
			UART1 = 0;
			
			while(i<package_len){
				Lab_receive[i]=buff1[i];
				//put_char(Lab_receive[i]);
				i++;
			}
			memset(buff1,0,50);	
			i=0;
			
			//Bestemmer hvilken fane beskeden er sendt fra
			type = Lab_receive[4];	//Burges til at opdatere lokale variabler
			msg_type = type;		//Bruges til at bestemme hvor retur beskeden skal sendes hen
			
		}
		
	////////////////////////////////////////
	//     Behandler data fra labview     //
	////////////////////////////////////////	
		switch(type){
			case Generator :
				priority = Generator;
				button = Lab_receive[5];
				
				type = Default;
				break;
				
			case Oscilloskop :
				sample = (int)((Lab_receive[5]<<8)+Lab_receive[6]);
				record_len = (int)((Lab_receive[7]<<8)+Lab_receive[8]);
				put_char((char)sample);
				//Bestemmer sample raten
				if(sample<4){
					sample = 4;
				}
				set_samplerate(sample);
				
				//Bestemmer Rekord length afhængig af sample rate
				min_record = ((7*(long)sample)/(-sample+11520)+1);
				
				if(record_len<=min_record){
					record_len = min_record;
				}
				put_char((char)min_record);

				//Sætter FIFO bufferen tilbage til start
				front = 0;
				rear = -1;
				itemCount = 0;
				
				type = Default;
				break;
				
			case Bodeplot :
				bodeplot();
				type = Default;
			break;
		}

		//Vælger hvad der skal sendes til labview
		if(isTXintr_enabled()==False){
			switch(priority){
				case Generator :
					switch(button){
					
					case Enter :
						Generator_data[menu+1]=Lab_receive[6];
						
						if(menu == 0 && Lab_receive[6]>4){
							Generator_data[menu+1] = 4;
						}
						
						SPI_send(menu, Generator_data[menu+1]);
						
						break;
					case Select :
						menu++;
						if(menu == 3)
						{
							menu = 0;
						}
						Generator_data[0] = menu;
						break;
					case RunStop:
						
						if(state==1){
							Generator_data[4] = 0;
							state = 0;
						}
						else{
							Generator_data[4] = 1;
							state = 1;
						}
						SPI_send(0x03, Generator_data[4]);
						break;
					case Reset :
						Generator_data[0] = 0; // menu
						Generator_data[1] = 0; // shape
						Generator_data[2] = 0; // amplitude
						Generator_data[3] = 0; // Frequency
						Generator_data[4] = 0; // Run/Stop
						SPI_send(0x00, 0);
						SPI_send(0x01, 0);
						SPI_send(0x02, 0);
						SPI_send(0x03, 0);
						menu = 0; //hvilken menu man er i, i generator tabben
						break;
					}
					priority = Oscilloskop;
					EN_intr_TX();
					break;
				case Oscilloskop :
					if(itemCount >= record_len && isTXintr_enabled()==False){
						EN_intr_TX(); //Enabler TXInt når vi er klar til at sende
					}
				break;
			}
		}
		
	/*	
		for(k=0;k<30000;k++){
			
			for(o=0;o<3;o++)
			{
				SPI_send(o, 6+k);
			}
			
		}
		put_char('K');
		
		*/
		
		
		
	}//end loop
}//end main


ISR(USART1_RX_vect){
	static unsigned int i = 0, turn=0;
	static char check=1;
	
	//Modtager data fra Labview
	
	switch(turn){
		case 0 :
			if(0x55 == UDR1){
				buff1[i] = UDR1;
				turn++;
			}
			break;
		case 1 :
			if(0xAA == UDR1){
				buff1[i] = UDR1;
				turn++;
			}
			else{
				i=-1;
				turn = 0;
			}
			break;
		case 2 :
			buff1[i] = UDR1;
			if(i==3){
				package_len = (int)(buff1[2]<<8)+buff1[3];
				turn++;
			}
			break;
		case 3 :
			buff1[i] = UDR1;
			if(i==package_len-3){
				turn++;
			}
			break;
		case 4 :
			buff1[i] = UDR1;
			check = buff1[i] ^ ((Checksum(package_len-7, buff1[4], buff1+5)&0xFF00)>>8);
			if(check == 0){
				turn++;
			}
			else{
				i=-1;
				turn = 0;
			}
			break;
		case 5 :
			buff1[i] = UDR1;
			check = buff1[i] ^ (Checksum(package_len-7, buff1[4], buff1+5)&0x00FF);//Vi trækker 7 fra for at få den aktuelle længde af dataen og ligger 5 til Buff1 da der er de første værdier er prædefineret i funktionen
			if(check == 0){
				UART1 = 1;
				i=-1;
				turn=0;
			}
			else{
				i=-1;
				turn = 0;
			}
			break;
	}
	
	i++;	
}

ISR(USART1_TX_vect){
	sei();
	static int i=0, turn=0, msg_length;
	static unsigned char data_out[MAX];
	
	//Bestemmer længden af den besked der skal sendes
	if(msg_type == Generator){
		msg_length = 5;
	}
	else if(msg_type == Oscilloskop){
		msg_length = record_len;
	}
	else if(msg_type == Bodeplot){
		msg_length = 255;
	}
	//Sender data til Labview
	switch(turn){	
		case 0 :
			turn++;
			UDR1 = 0x55;
			break;
		case 1 :		
			turn++;
			UDR1 = 0xAA;
			break;
		case 2 :
			turn++;
			UDR1 = ((msg_length+7)&0xFF00)>>8;//Der bliver lagt 7 til msg_length da der også er sync,length, type og chksum bytes 
			break;
		case 3 :
			turn++;
			UDR1 = ((msg_length+7)&0x00FF);
			break;
		case 4 :
			turn++;
			UDR1 = msg_type;
			break;
		case 5 :
			if(msg_type == Generator){
				data_out[i] = Generator_data[i];	
			}
			else if(msg_type == Oscilloskop){
				data_out[i] = removeData();
			}
			else if(msg_type == Bodeplot){
				data_out[i] = amp[i]; 
			}
				
			UDR1 = data_out[i];
			
			i++;
			if(i==msg_length){
				turn++;
				i=0;
			}
			break;
		case 6 :
			turn++;
			UDR1 = ((Checksum(msg_length, msg_type, data_out))&0xFF00)>>8;
			break;
		case 7 :
			turn=0;
			UDR1 = ((Checksum(msg_length, msg_type, data_out))&0x00FF);
			DIS_intr_TX();
			msg_type = Oscilloskop;
			break;
	}
}


//TEST UART0
ISR(USART0_TX_vect){
	sei();
	static int i=0, turn=0;
	static unsigned char checksum=0, data_out;
	
	switch(turn){
		case 0 :
			turn=1;
			checksum ^= 0x55^0xAA^(((record_len+7)&0xFF00)>>8)^((record_len+7)&0x00FF)^0x02;
			UDR0 = 0x55;
		break;
		case 1 :
			turn=2;
			UDR0 = 0xAA;
			break;
		case 2 :
			turn=3;
			UDR0 = ((record_len+7)&0xFF00)>>8;
			break;
		case 3 :
			turn=4;
			UDR0 = (record_len+7)&0x00FF;
			break;
		case 4 :
			turn=5;
			UDR0 = 0x02;
			break;
		case 5 :
			data_out = removeData();
			checksum ^= data_out;
			UDR0 = data_out;
			i++;
			if(i==record_len-1){
				turn = 6;
				i=0;
			}
		break;
			case 6 :
			turn=7;
			UDR0 = 0x00;
			break;
		case 7 :
			turn=0;
			UDR0 = checksum;
			DIS_intr_TX();
			checksum=0;
			break;
	}
}

volatile int Checksum(int len, char type, unsigned char *data){
	int unsigned chksum = 0, i;
	unsigned char x, pre_data[] = {0x55,0xAA,(((len+7)&0xFF00)>>8),((len+7)&0x00FF),type};
	
	switch(checksum){
		case ZERO16 : 
			chksum = 0;
			break;
		case LRC8 :
			for(i=0;i<5;i++){
				chksum ^= (int)pre_data[i];
			}
			for(i=0;i<len;i++){
				chksum ^= (int)data[i];
			}
			break;
		case CRC16 : 
			chksum = 0xFFFF;
			
			for(i=0;i<5;i++){
				x = chksum >> 8 ^ (int)pre_data[i];
				x ^= x>>4;
				chksum = (chksum << 8) ^ ((unsigned int)(x << 12)) ^ ((unsigned int)(x <<5)) ^ ((unsigned int)x);
			}
			for(i=0;i<len;i++){
				x = chksum >> 8 ^ (int)data[i];
				x ^= x>>4;
				chksum = (chksum << 8) ^ ((unsigned int)(x << 12)) ^ ((unsigned int)(x <<5)) ^ ((unsigned int)x);
			}
			break;
	}
	return chksum;
}


void bodeplot(){
	int i;
	SPI_send(0x00, 0x03);
	SPI_send(0x01, 0xFF);
	SPI_send(0x03, 0x01);
	set_samplerate(60000);
	for(i=0;i<255;i++){
		SPI_send(0x02,i+1);
		amp[i-1] = amplitude();
	}  
	
	EN_intr_TX();
}

char amplitude(){
	front = 0;
	rear = -1;
	itemCount = 0;
	int i;
	char datain = 0, max = 0,min = 0xFF;
	for(i=0;i<5000;i++){
		while (itemCount != 0);
		datain = removeData();
		if (datain > max){
			max = datain;
		}
		if (datain < min){
			min = datain;
		}
	}
	return max-min;
}