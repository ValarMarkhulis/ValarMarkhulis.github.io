/*
 * ADC_timer.c
 *
 * Created: 6/10/2017 13:54:26
 *  Author: Christian
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Header.h"



///////////////////////
// TIMER 1 FUNCTIONS //
///////////////////////

void init_timer1(unsigned int sample_rate){
	OCR1A = 250000/sample_rate; // udregner compareværdi
	OCR1B = 1;
	TCCR1B |= (1<<WGM12); // CTC mode
	TCCR1B |= (1<<CS11)|(1<<CS10); //CLK/64
	TIMSK1 |= (1<<OCIE1B); // comparematch interrupt enable
}


///////////////////
// ADC FUNCTIONS //
///////////////////

void init_ADC_interrupt_AREF(unsigned char channel){
	ADMUX  |=(channel&0x07);
	ADCSRA |= (1<<ADEN); // ADC Enable side 292
	ADCSRA |= (1<<ADPS2); //skaller med faktor 16, side 293.
	ADCSRA |= (1<<ADIE); // ADC Interrupt Enable
	ADCSRA |= (1<<ADSC); // start conversion
	ADCSRA |= (1<<ADATE); // auto trigger mode
	ADCSRB |= (1<<ADTS2)|(1<<ADTS0); // timer 1 compare match b trigger
	ADMUX  |= (1<<ADLAR); // "ADC Left Adjust Result" sat højt side 290.
}

void set_samplerate(unsigned int sample_rate){
	OCR1A = 250000/sample_rate;
}

////////////////////////
//  SERVICE ROUTINES  //
////////////////////////

ISR(ADC_vect){
	insert(ADCH);//Indsætter en ADC måling i FIFO bufferen
}

ISR (TIMER1_COMPB_vect){	
}