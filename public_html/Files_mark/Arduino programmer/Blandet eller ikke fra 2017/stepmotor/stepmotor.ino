// Definitionerne til stepmotor (og motorstyring)
int updownpin = 6;  // Updown Pin
int pulspin =  8; // Pulsens Pin
int pushbutton = 13; // Knappen til op
int pushbutton2 = 12; // Knappen til ned

//High og Low til digital pins
int stateL = LOW;
int stateH = HIGH;

//********************************************************//
// Definitioner til temperatur sensor
long interval = 1000;    // interval på 1 sek som vi måler i. 
long b = 0;    // start interval hvor vi måler
float c1;
float temp1;
int termoA;    // det første input fra temp.sensor
int termoB;    // det andet input fra temp.sensor
//********************************************************//

void setup()
{
  Serial.begin(9600);
  pinMode(updownpin, OUTPUT);
  pinMode(pulspin, OUTPUT); 
  pinMode(pushbutton, INPUT);
  pinMode(pushbutton2, INPUT);
}

void loop()
{
//********************************************************//
  // Temperatur sensor del
  
  unsigned long s = millis();    // definer et nyt variabel s som skal tælle op
 
   if (s - b > interval){    // vi laver en if statment som skal kun tælle op nå der var gået 1 sec
     termoA = analogRead(A1);    // vi aflæser det første input 
     termoB = analogRead(A0);    // vi aflæser det andet input 
     c1 = float(termoA)/204.6-float(termoB)/204.6;
     
     /*
     **********************************************************************************
     Forklaring på c1:
     - vi omdanner variable til at være lige med volt og
     - derefter substragere vi med hinanden for en af dem skal
     - være negativ, efter vores opsætning.
     - Fra databladet ved vi, at 1 grad C = 0.001V så vi omdanner vores variabel til deg C.
     **********************************************************************************
     */
     
     temp1 = c1*100;    // vi gemmer vores første temp.
     }
     
  Serial.print("Temp.sensor 1: ");
  Serial.println(temp1, DEC);
  delay(1000);
//********************************************************//     
     
//********************************************************//
  // Stepmotor del  
  
  
  // Tone er en funktion der generere et firkantet signal. 
  // Den virker på den her måde: tone(pin, frequency)
  // frequency skal defineres i Hz, og funktionen kan min.
  // generere 31Hz.
  
///////////// Knap til brugeren
int BSS = digitalRead(pushbutton); // Læs værdien på pin.
int BS = digitalRead(pushbutton2); // Læs værdien på pin.


 if(BS != 0) {
  digitalWrite(updownpin, stateH);
  tone(pulspin, 56, 2000);
  Serial.println("Knap 1 trykkes : Gardinet gaar op");
  delay(2000);
  }
  
  if(BSS != 0) {
  digitalWrite(updownpin, stateL);
  tone(pulspin, 56, 2000);
  Serial.println("Knap 2 trykkes : Gardinet gaar ned"); 
  delay(2000);
  }

}

