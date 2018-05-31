#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

//variabler til wakeup light
int ledPin = 5;    
//int knap = 2;
////////////////////////////////////////////////////////
//Variabler til temperatursensor
long b = 0;    // start interval hvor vi måler
float c1;
float temp1;
int termoA;    // Output fra temp.sensor til arduino.
////////////////////////////////////////////////////////
//variabler til uret (tælle tid)
unsigned long foer;
unsigned long tidSek;
unsigned long tidMin;
unsigned long tidTime;
unsigned long interval;
////////////////////////////////////////////////////////
//variabler til hver kontakt
const int setAlarm = 7;
const int setTimer = 6;    //setTime i arduino er allerede en funktion, derfor setTimer.
const int setMin = 4;
////////////////////////////////////////////////////////
//variabler til alarmen
int setAlarmState = 0;
int setTimerState = 0;
int setMinState = 0;
int lastSetAlarmState = 0;
int lastSetTimerState = 0;
int lastSetMinState = 0;

int alarmTime;
int alarmMin;

int alarmStart = 0;

boolean alarmMode;
////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  //*************************************************
  // setup af LCD display. Antal kolonner og rækker.
  lcd.begin(16, 4);
  
  // 16 er antal pladser i hver række.
  // 4 er antal rækker totalt.
  // I arduino starter array indekser i 0.
  // Første række er derfor 0.
  // Første kolonne er derfor 0.
  //*************************************************
  
  pinMode(setAlarm, INPUT);
  pinMode(setTimer, INPUT);
  pinMode(setMin, INPUT);
//  pinMode(knap, INPUT);
  
  alarmMode = false;
  alarmTime = 0;
  alarmMin = 0;
  
  //Setup til ur delen
  //Vi sætter sekunder, minutter og timer til 0.
  interval = 1000;
  foer = 0;
  tidSek = 0;
  tidMin = 0;
  tidTime = 0;
}

void loop() {
  unsigned long nu =  millis();
  unsigned long s = millis();
  
  setAlarmState = digitalRead(setAlarm);
  setTimerState = digitalRead(setTimer);
  setMinState = digitalRead(setMin);
  
 //int knaptryk = digitalRead(knap); // Læs værdien på pin.
  digitalWrite(ledPin, LOW);
  
  
/*  if(knaptryk != 0) {
    Serial.println("Knappen trykkes");
    // fade in from min to max in increments of 5 points:
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue++) {
      //sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      //delay(50);
      
    }
    Serial.println("Lyset slukker");
    analogWrite(ledPin, 0);
  } */
  
  if (s - b > interval){    // vi laver en if statment som skal kun tælle op nå der var gået 1 sec
     termoA = analogRead(A0);    // vi aflæser det første input 
     
     /*
     **************************************************************************************
     Forklaring på temp1:
     - vi omdanner variable til at være lige med volt og
     - derefter substragere vi med hinanden for en af dem skal
     - være negativ, efter vores opsætning.
     - Fra databladet ved vi, at 1 grad C = 0.001V så vi omdanner vores variabel til deg C.
     - Ved forstærnking har vi forstærket med en faktor 5.
     - Når vi så laver analogRead(A0), om til grader dividere vi med 204.6, og ganger med 20.
     **************************************************************************************
     */
  
     temp1 = (termoA/204.65)*20;    //vi laver værdien fra termoA om til temperatur i grader celsius.
   }
   
   if(setAlarmState != lastSetAlarmState) {
      if(setAlarmState == HIGH) {
        if(!alarmMode) {
          alarmMode = true;  
        }
        else {
          alarmMode = false;  
        }
      }
   }
   lastSetAlarmState = setAlarmState;
   if(alarmMode) {
     lcd.setCursor(0,0);
     lcd.print("   --Alarm--      ");
        
     lcd.setCursor(0,1);
     lcd.print("                  ");
     lcd.setCursor(0,1);
     lcd.print("Time: ");
     lcd.print(alarmTime);
        
     lcd.setCursor(0,2);
     lcd.print("                  ");
     lcd.setCursor(0,2);
     lcd.print("Min:  ");
     lcd.print(alarmMin);
     lcd.setCursor(0,2);
     
     lcd.setCursor(0,3);
     lcd.print("                  ");
        
     if(setTimerState != lastSetTimerState) {
       if(setTimerState == HIGH) {
         Serial.print(alarmTime);
         Serial.print(" ");    
         alarmTime++;  
         if(alarmTime>=24) {
           alarmTime = 0;  
         }      
       }  
     }
     lastSetTimerState = setTimerState;
        
     if(setMinState != lastSetMinState) {
       if(setMinState == HIGH) {
         Serial.println(alarmMin);
           alarmMin++;
           if(alarmMin>=60) {
             alarmMin = 0;  
           } 
       }  
     }
     lastSetMinState = setMinState;
             
    }
    //Printer andet end at sætte alarmen.
    if(!alarmMode) {
      if(nu-foer>interval) {
        foer = nu;
          
        //printer tid ud på displayet
        lcd.setCursor(0,0);
        lcd.print("Tid:   ");
        lcd.print(tidTime);
        lcd.print(":");
        lcd.print(tidMin);
        lcd.print(":");
        lcd.print(tidSek);
        //dette sidste print, er for at
        //undgå at printe tegn fra alarm sætning
        //så vi rydder de sidste pladser.
        lcd.print("   ");
        
        //printer på 2. linje.
        //Her printer vi tidspunktet,
        //som brugeren har sat alarmen til.
        lcd.setCursor(0, 1);
        lcd.print("Alarm: ");
        lcd.print(alarmTime);
        lcd.print(":");
        lcd.print(alarmMin);
        
        //printer på 3. linje
        //Her printer vi indetemperaturen.
        lcd.setCursor(0,2);
        lcd.print("Inde:  ");
        lcd.print(temp1);
        lcd.print("\337C");
        
        //printer på 4. linje
        //her printer vi ude temperaturen,
        //som vi modtager gennem Xbee fra en anden arduino.
        lcd.setCursor(0,3);
        lcd.print("Ude: ");
        
        //Her tæller vi variablen tidSek op,
        //variablen er antal sekunder.
        tidSek++;
      }
      
      //tæller minutter. Hvis variablen tidSek
      //bliver større eller lig med 60,
      //sætter vi tidSek lig med nul.
      if(tidSek>= 60){
        tidMin++;
        tidSek = 0;
      }
      
      //tæller timer. Hvis variablen tidMin
      //bliver større eller lig med 60,
      //sætter vi den til nul (reset).
      if(tidMin>= 60){
       tidTime++;
       tidMin = 0;
      }
      
     //Hvis timer når 24, bliver den sat til 0.
     //På uret vil midnat være 0:0:0.
     //Her resetter vi variablen timer.
     if(tidTime>= 24){
       tidTime++;
       tidTime = 0;
     }
      
  }//slut på if(!alarmMode)
  
  //funktion til alarm delen.
  //tæller antal sekunder efter programmet er startet.
  alarmStart = (s/1000);
  if(tidTime==alarmTime && tidMin==alarmMin && alarmStart>=65) {
    tone(3, 150, 5000);
    // fade in from min to max in increments of 5 points:
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue++) {
      //sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      delay(50);
      
    }}
  
  
}//slut på void loop

