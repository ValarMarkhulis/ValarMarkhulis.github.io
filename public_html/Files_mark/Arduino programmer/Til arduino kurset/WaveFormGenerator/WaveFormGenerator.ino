#include <TimerOne.h>

#include "Waveforms.h" 
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int knap0 = 20; // Waveform
const int knap1 = 21; // Frekvens
volatile boolean toggle0 = 0;
volatile boolean toggle1 = 0;
volatile sigmode = 0; // 0 for square and 1 for sinus

volatile int firkant = 0;
volatile int sinsignal = 0;

const int a = 49; // a PL0
const int b = 48; // b PL1
const int c = 47; // c PL2
const int d = 46; // d PL3
const int e = 45; // e PL4
const int f = 44; // f PL5
const int g = 43; // g PL6 
const int h = 42; // h PL7

int i = 0;
int j = 0;
int delay_fre = 0;


void setup() {
  Timer1.initialize(100000);
  Timer1.attachInterrupt( timerIsr );   // attach the service routine here

  
  for(int i = 0;i<9;i++){
  pinMode(i,OUTPUT);
  }
  
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Waveform 1 valgt");
  lcd.setCursor(0, 1);
  lcd.print("Frekvens 1 valgt");

  pinMode(A0,INPUT);
  
  pinMode(knap0,INPUT);
  pinMode(knap1,INPUT);
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT); // built-in led
  attachInterrupt(digitalPinToInterrupt(knap0), knap0kald , RISING);  // Interrupt attached to the button connected to pin 20
  attachInterrupt(digitalPinToInterrupt(knap1), knap1kald , RISING);
  
}

void loop() {

/*
for(int ii = 0; ii < 120; ii++){
  PORTL = waveformsTable[3][ii];
  int test = analogRead(A0);
  Serial.println(test);
  delay(delay_fre);
}

*/
  



/*
  if(i > 127){
    PORTL = 255;
    if(i == 255){
      i = 0;
    }
  }else{
    PORTL = 0;
  }
  i++;
  */
  
  //int test = analogRead(A0);
 // Serial.println(analogRead(A0));
}

void knap0kald(){
  lcd.setCursor(0, 0);
  
  if (toggle0 == 1){
      sigmode = 0;
      toggle0 = 0;
      digitalWrite(LED_BUILTIN, HIGH);
      lcd.print("Waveform 1 valgt");
  }else{
    sigmode
    toggle0 = 1;
    digitalWrite(LED_BUILTIN, LOW); 
    lcd.print("Waveform 2 valgt");    
  }

  
}

void knap1kald(){
  lcd.setCursor(0, 1);

  if (toggle1 == 1)
  {
      toggle1 = 0;
      digitalWrite(LED_BUILTIN, HIGH);
      lcd.print("Frekvens 1 valgt");
      Timer1.initialize(100000);
      delay_fre = 0;  
  }else
  {
    toggle1 = 1;
    digitalWrite(LED_BUILTIN, LOW); 
    lcd.print("Frekvens 2 valgt");
    Timer1.initialize(1000000); 
    delay_fre = 20;
  }
  
  
}


void timerIsr(){          // Custom ISR Timer Routine/

  if(sigmode == 0){
      if(firkant == 0){
        PORTL = waveformsTable[3][0];
        firkant = 1;
      }else{
        PORTL = waveformsTable[3][80];
        firkant = 0;
      }   
  }else if(sigmode == 1){
      sinsignal++;
      if(sinsignal > 110){
        sinsignal = 0;
      }
      PORTL = sine_wave[sinsignal];
  }

  
  
  /*
  sinsignal++;
  if(sinsignal > 110){
    sinsignal = 0;
  }
  PORTL = sine_wave[sinsignal];
  */
  
}


