/*
 * Header.h
 *
 * Created: 6/10/2017 13:51:05
 *  Author: Christian
 */ 


#ifndef HEADER_H_
#define HEADER_H_

///////////////////////
//     ADC_timer     //
///////////////////////

void init_timer1(unsigned int sample_rate);
void init_ADC_interrupt_AREF(unsigned char channel);
void set_samplerate(unsigned int sample_rate);

//////////////////////
//       UART       //
//////////////////////

unsigned int record_len;

void init_uart0(int baud);
void init_uart1(int baud);
void EN_intr_TX();
void DIS_intr_TX();
char isTXintr_enabled();
void put_char(char data);

///////////////////////
//        SPI        //
///////////////////////

void SPI_MasterInit();
unsigned char SPI_send(unsigned char addr, unsigned char data);

//////////////////////
//       FIFO       //
//////////////////////

volatile unsigned int itemCount;
volatile int front;
volatile int rear;

//Bestemmer størrelsen på FIFO bufferen og output arrayet til labview
#define MAX 2000

void insert(char data);
char removeData();

#endif /* HEADER_H_ */