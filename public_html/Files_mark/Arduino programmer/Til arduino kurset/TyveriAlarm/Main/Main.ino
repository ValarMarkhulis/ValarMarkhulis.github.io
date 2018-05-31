// DS1302_Serial_Easy (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Adopted for DS1302RTC library by Timur Maksimov 2014

#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>

// Set pins:  CE, IO,CLK
DS1302RTC RTC(27, 29, 31);

const int wakeup = 6;
const int workstart = 9;
const int workend = 18;
int tid0 = 0;
int tid1 = 0;

void setup()
{

  // Setup Serial connection
  Serial.begin(9600);
  
  Serial.println("RTC module activated");
  Serial.println();
  delay(500);
  
  if (RTC.haltRTC()) {
    Serial.println("The DS1302 is stopped.  Please run the SetTime");
    Serial.println("example to initialize the time and begin running.");
    Serial.println();
  }
  if (!RTC.writeEN()) {
    Serial.println("The DS1302 is write protected. This normal.");
    Serial.println();
  } 
}

void loop()
{
  tmElements_t tm; // The read(tmElements_t &tm) method reads the current time from the RTC and returns it as a tmElements_t structure.
  RTC.read(tm); // RTC modulet laver et kald om at få opdateret tiden
  
  tid0 = tm.Minute; // Lægger aktuel time i tid0 variablen
  if( tid0 != tid1){ // Kontroller om der er gået en time
    tid1=tid0;
    Serial.print("An hour has passed, checking status of alarm...\n");
    
    if(tm.Hour < wakeup || (tm.Hour >= workstart && tm.Hour < workend) ){
        Serial.println("The alarm is Activated");
    }else{
        Serial.println("The alarm is Deactivated");
    }
  }
  
  if (! RTC.read(tm)) {
    Serial.print("Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.println("");
  }

  
  // Wait one second before repeating:
  delay (1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}
