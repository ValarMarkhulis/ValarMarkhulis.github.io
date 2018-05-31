/* Programmet er kode der er genereret udfra Atmels "CodeWizardAVR" til
 * at kunne sende og modtage Serial kommunikation mellem 2 arduinoer 
 * vha. UART1. 
 *   
 * Gruppen bestod af: Christian Mark og Mads Astrup
 * 
 * Dette program skal kører på Arduino A, som er den arduino som får bruger input
 * fra Serial monitoren.
 */

#define DATA_REGISTER_EMPTY (1<<UDRE1)
#define RX_COMPLETE (1<<RXC1)
#define FRAMING_ERROR (1<<FE1)
#define PARITY_ERROR (1<<UPE1)
#define DATA_OVERRUN (1<<DOR1)

// Get a character from the USART1 Receiver
#pragma used+
char getchar1(void)
{
unsigned char status;
char data;
while (1)
      {
      while (((status=UCSR1A) & RX_COMPLETE)==0);
      data=UDR1;
      if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){
         return data;
      }

      }
}

#pragma used-

// Write a character to the USART1 Transmitter
#pragma used+
void putchar1(char c)
{
while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
UDR1=c;
}

#pragma used-

// Standard Input/Output functions
#include <stdio.h>

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  // USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 9600
UCSR1A=(0<<RXC1) | (0<<TXC1) | (0<<UDRE1) | (0<<FE1) | (0<<DOR1) | (0<<UPE1) | (0<<U2X1) | (0<<MPCM1);
UCSR1B=(0<<RXCIE1) | (0<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
UCSR1C=(0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCPOL1);
UBRR1H=0x00;
UBRR1L=0x67;

}

void loop() { 

  // Send fra Serial Monitoren og modtag karakter på modtager
  char test = '1';
  if(Serial.available() > 0){
    test = Serial.read();
    putchar1(test);
  }
  
}
