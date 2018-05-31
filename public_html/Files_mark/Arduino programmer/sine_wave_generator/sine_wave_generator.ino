/* Programmet er lavet af Mads Astrup og Christian Mark.
 * Programmet bruger timer3 og analogWrite() funktionen til at kunne lave en sinus og et firkantet signal
 * Følgende HW er brugt:
 - LCD display : Til at vise hvilken "Mode" og "Frekvens" der er valgt
 - 2 Push knapper : Styrer hvilken "Mode" og "Frekvens" der skal bruges
 * 
*/
#define SAMPLESIZE 256
#include <LiquidCrystal.h>



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 6, 4, 3, 2);

const int knap0 = 20; // Waveform
const int knap1 = 21; // Frekvens

volatile boolean toggle0 = 0; // Bruges til at holde styr på knap0
volatile boolean toggle1 = 0; // Bruges til at holde styr på knap1

//table of sinewave values by genericpenguin at:https://gist.github.com/genericpenguin/6bfaeceeb86498c6065c#file-synth-cpp.
//it is values from a plotted sinus wave in the spectrum between 0 and 255.
volatile const int sigTable[2][SAMPLESIZE]= {{
  127,  130,  133,  136,  139,  143,  146,  149,  152,  155,  158,  161,  164,  167,  170,  173,
  176,  178,  181,  184,  187,  190,  192,  195,  198,  200,  203,  205,  208,  210,  212,  215,
  217,  219,  221,  223,  225,  227,  229,  231,  233,  234,  236,  238,  239,  240,  242,  243,
  244,  245,  247,  248,  249,  249,  250,  251,  252,  252,  253,  253,  253,  254,  254,  254,
  254,  254,  254,  254,  253,  253,  253,  252,  252,  251,  250,  249,  249,  248,  247,  245,
  244,  243,  242,  240,  239,  238,  236,  234,  233,  231,  229,  227,  225,  223,  221,  219,
  217,  215,  212,  210,  208,  205,  203,  200,  198,  195,  192,  190,  187,  184,  181,  178,
  176,  173,  170,  167,  164,  161,  158,  155,  152,  149,  146,  143,  139,  136,  133,  130,
  127,  124,  121,  118,  115,  111,  108,  105,  102,  99,   96,   93,   90,   87,   84,   81,
  78,   76,   73,   70,   67,   64,   62,   59,   56,   54,   51,   49,   46,   44,   42,   39,
  37,   35,   33,   31,   29,   27,   25,   23,   21,   20,   18,   16,   15,   14,   12,   11,
  10,   9,    7,    6,    5,    5,    4,    3,    2,    2,    1,    1,    1,    0,    0,    0,
  0,    0,    0,    0,    1,    1,    1,    2,    2,    3,    4,    5,    5,    6,    7,    9,
  10,   11,   12,   14,   15,   16,   18,   20,   21,   23,   25,   27,   29,   31,   33,   35,
  37,   39,   42,   44,   46,   49,   51,   54,   56,   59,   62,   64,   67,   70,   73,   76,
  78,   81,   84,   87,   90,   93,   96,   99,   102,  105,  108,  111,  115,  118,  121,  124},
   
 {255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,  255, 
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  
  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,}
};

volatile int i = 0;
volatile int mode = 0;
volatile int gPin = 5;

void setup() {
  
  TCCR3B = TCCR3B & 0x00; //reset prescaler for timer 3.
  TCCR3B |=  0x01; // prescaler værdi: clk/1
  //TCCR3B |= 0x02; // prescaler værdi: clk/8
  //TCCR3B |= 0x03; // prescaler værdi: clk/64
  //TCCR3B |= 0x04; // prescaler værdi clk/256
  //TCCR3B |= 0x05; // prescaler værdi clk/1024
  
  bitSet(TCCR3B, WGM32);  //enable fast PWM mode 8-bit
  bitSet(TIMSK3, TOIE3);  //sets "TOIEn" in register "TIMSKn" to one. This bit enables timer overflow interrupts.
  
  analogWrite(gPin, 271); //sets gPin to PWM output
  Serial.begin(9600);
  
  pinMode(knap0,INPUT); // Sætter knap0 der styre waveform til input
  pinMode(knap1,INPUT); // Sætter knap1 der styre frekvens til input
  
  //attachInterrupt(digitalPinToInterrupt(knap0), knap0kald , RISING);  // Interrupt sat på init 3 som er digital pin 20
  attachInterrupt(digitalPinToInterrupt(knap1), knap1kald , RISING);  // Interrupt sat på init 2 som er digital pin 21

  //Initialiser LCD displayet ved at skrive std. værdier til det.
  lcd.begin(16, 2);
  lcd.print("Waveform 1 valgt");
  lcd.setCursor(0, 1);
  lcd.print("Frekvens 1 valgt");

  
TIMSK3 |= 0b00000001;
//eller
TIMSK3 |= 0x01;
  
  sei(); // Enabler global interrupt
}
  

void loop() {

}

ISR(TIMER3_OVF_vect){ // Interrupt Service rutine som venter på et overflow på Timer3
  cli(); // Disabler global interrupt

  // PWM signalet på port 5 bruger OCR3A som "Output Compare register".
  // Så her skrives direkte til registeret, istedet for at bruge analogWrite()
  OCR3A = sigTable[mode][i]; 
  
  i+=1; // incrementer i, så næste gang et interrupt sker vil OCR3A 
  if (i >= SAMPLESIZE - 1){ // Hvis 'i' som indexere arrayet sigTable[] når til sidste element
    i = 0;
  }
  sei(); 
}

ISR(knap0kald){ // Knaptryk som justere waveform
  cli();
  delayMicroseconds(30000); // Debounce pause
  
  lcd.setCursor(0, 0);
  
  if (toggle0 == 1){
      toggle0 = 0;
      lcd.print("Waveform: sinus");
      mode = 0; // Sætter Waveform til sinus
  }else{
    toggle0 = 1;
    lcd.print("Waveform: square"); 
    mode = 1; // Sætter Waveform til square
  }

  sei();
}

void knap1kald(){ // Knaptryk som justere waveform
  cli();
  delayMicroseconds(300000);
  
  lcd.setCursor(0, 1);

  if (toggle1 == 1)
  {
    toggle1 = 0;
    
    lcd.print("Frekvens 1 valgt");
    TCCR3B &= 0xF8; // Clear
    TCCR3B |= 0x01; // prescaler værdi clk/1
      
  }else
  {
    toggle1 = 1;
    TCCR3B &= 0xF8;
    TCCR3B |=  0x02; // prescaler værdi: clk/8
    lcd.print("Frekvens 2 valgt"); 
    
  }
  
  sei();
}
