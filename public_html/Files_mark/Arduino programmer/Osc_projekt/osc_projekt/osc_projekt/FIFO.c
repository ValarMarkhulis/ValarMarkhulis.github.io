/*
 * FIFO.c
 *
 * Created: 6/10/2017 14:58:18
 *  Author: Christian
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Header.h"

  
volatile unsigned char FIFO[MAX]; //Størrelsen af FIFO bestemmes i headeren
volatile int front = 0;
volatile int rear = -1;
volatile unsigned int itemCount = 0;

void insert(char data){

	if(itemCount != MAX) {
		
		if(rear == MAX-1) {
			rear = -1;
		}
		FIFO[++rear] = data;
		itemCount++;
	}
}
char removeData(){
	char data = FIFO[front++];
	
	if(front == MAX){
		front = 0;
	}
	itemCount--;
	return data;
}