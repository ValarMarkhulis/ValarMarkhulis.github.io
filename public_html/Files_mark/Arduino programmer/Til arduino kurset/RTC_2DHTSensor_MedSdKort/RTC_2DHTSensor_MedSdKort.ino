/* Programmet er "sat sammen" af Christian Mark og Mads Astrup, men
 * RTC delen er efterfølgende blevet implementeret af Christian Mark.
 * 
 * Programmet består af 3 dele:
 * 
 * Et RTC modul - Koden er kopieret DIREKTE og modificeret fra exampel sketchen "SetSerial" fra Arduino biblioteket "DS1302RTC"
 * som kan hentes og installeres fra Arduinos "library Manager" eller findes på følgende github
 * side: https://github.com/JChristensen/DS3232RTC. Biblioteket implementere både opsætning og aflæsning af RTC modulet. 
 * Programmet bruger læsning af dato og tidspunkt fra RTC modulet til at skrive de oplysninger i data filerne på SD kortet.
 * 
 * Et SD kort - Koden er kopieret fra sidste uges opgave, men er til at starte med skrevet ud fra eksemplerne i "SD"
 * kategorien. SD kortet bruges til at modtage sensoraflæsninger fra 2 sensore og aflæsningen fra RTC modulet. Arduinoen 
 * tager hver sensorlæsning, lægger det ned i 2 forskellige output filer og påføre hvert datapunkt med dato og tidspunkt for aflæsning. 
 * 
 * Sidste del er 2 fysiske DHT sensore som er "Humidity & Temperature" sensore, som giver et digital output der kan aflæses via 
 * DHT biblioteket vi har fået udleveret. 
 */


// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

/////// 
#include <SPI.h>
#include <SD.h>

File myOutputFile1;
File myOutputFile2;

int buffervar = 0;
//////


////////////////////////////// RTC inkludering og RTC initialisering
#include <DS1302RTC.h>
#include <Streaming.h>        //http://arduiniana.org/libraries/streaming/
#include <Time.h>             //http://playground.arduino.cc/Code/Time
#include <TimeLib.h>

// Set pins:  CE, IO,CLK
DS1302RTC RTC(27, 29, 31);
void RTC_begin(); // Funktion prototype

//////////////////////////////

#include "DHT.h"

#define DHTPINa 2     // what pin we're connected to
#define DHTPINb 4     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

DHT  DHTa(DHTPINa, DHTTYPE); // Definere den første sensor
DHT  DHTb(DHTPINb, DHTTYPE); // Definere den anden sensor
void readsensor(); // Funktions prototype til at aflæse sensorene

//// Globale variabler til aflæsning af sensor
// Sensor 1
float ha = 0;
float ta = 0;
float fa = 0;
float hia = 0;

//Sensor 2
float hb = 0;
float tb = 0;
float fb = 0;
float hib = 0;

void setup() {
  Serial.println("DHTxx test!");

  ////////////////////////// SD KORT
  cli();
  Serial.begin(9600);
  while(!Serial); //wait for serial to connect

   
  //initialize sd card  
  Serial.print("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }
   
  Serial.println("initialization of SD card done.");
  
  myOutputFile1 = SD.open("output1.txt", FILE_WRITE);
  myOutputFile2 = SD.open("output2.txt", FILE_WRITE);
  sei();
  //////////////////////////

  RTC_begin(); // Opsætning af RTC
 
  DHTa.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  //delay(300000); // 5 min
  //delay(59000); // (1 min) - (1 sekund)

  readsensor(); // Læs sensorværdierne og udskriv dem i Serial monitoren
  

/////////////////////////////////////////RTC Start
    static time_t tLast;
    time_t t;
    tmElements_t tm;

    //check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
    if (Serial.available() >= 12) {
        //note that the tmElements_t Year member is an offset from 1970,
        //but the RTC wants the last two digits of the calendar year.
        //use the convenience macros from Time.h to do the conversions.
        int y = Serial.parseInt();
        if (y >= 100 && y < 1000)
            Serial << F("Error: Year must be two digits or four digits!") << endl;
        else {
            if (y >= 1000)
                tm.Year = CalendarYrToTm(y);
            else    //(y < 100)
                tm.Year = y2kYearToTm(y);
            tm.Month = Serial.parseInt();
            Serial.print(" ");
            tm.Day = Serial.parseInt();
            tm.Hour = Serial.parseInt();
            tm.Minute = Serial.parseInt();
            tm.Second = Serial.parseInt();
            t = makeTime(tm);
      //use the time_t value to ensure correct weekday is set
            if(RTC.set(t) == 0) { // Success
              setTime(t);
              Serial << F("RTC set to: ");
              printDateTime(t);
              Serial << endl;
      }
      else
        Serial << F("RTC set failed!") << endl;
            //dump any extraneous input
            while (Serial.available() > 0) Serial.read();
        }
    }
    
    t = now();
    if (t != tLast) {
        tLast = t;
        printDateTime(t);
        Serial << endl;
    }
//////////////////////////////////////////// RTC Slut

///////////////////////////////////////// SD KORT Start

    // Skriver til Output filerne med fugtighed og temperatur
    if (myOutputFile1) {
        // Humidity
        myOutputFile1.print("\t");
        myOutputFile1.print(ha);
        myOutputFile1.print("\t");
        //Temperature 
        myOutputFile1.println(ta);
              
      // close the file:
      myOutputFile1.flush();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening output1.txt error 0");
    }
    
        if (myOutputFile2) {
        // Humidity
        myOutputFile2.print("\t");
        myOutputFile2.print(hb);
        myOutputFile2.print("\t");
        // Temperature
        myOutputFile2.println(tb);
      
      // close the file:
      myOutputFile2.flush();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening output2.txt error 0");
    }

///////////////////////////////////////////////////////////// SLUT SD KORT
} // Slut main


//////////////////////////////////RTC funktioner
//print date and time to Serial and Output files on SD card
void printDateTime(time_t t)
{
    printDate(t);
    Serial << ' ';
    myOutputFile1.print(" ");
    myOutputFile2.print(" ");
    printTime(t);
}

//print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}

//print date to Serial and to output files on SD card
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial << ' ' << monthShortStr(month(t))<< ' ' << _DEC(year(t)) << " -";
    
    ////////////////////// SD kort udskrvining af dato
    myOutputFile1.print(" ");
    myOutputFile1.print(monthShortStr(month(t)));
    myOutputFile1.print(" ");
    myOutputFile1.print(year(t));
    myOutputFile1.print(" -");    
    
    myOutputFile2.print(" ");
    myOutputFile2.print(monthShortStr(month(t)));
    myOutputFile2.print(" ");
    myOutputFile2.print(year(t));
    myOutputFile2.print(" -"); 

    ///////////////////////
}

//Print an integer in "00" format (with leading zero),
//followed by a delimiter character to Serial.
//Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10){ // Udskriv et 0, hvis værdien kun er 1 ciffer
    myOutputFile1.print("0");
    myOutputFile2.print("0");
    Serial << '0';
    }

    // Udskriv værdien til outputfilerne
    myOutputFile1.print(val);
    myOutputFile2.print(val);
    
    Serial << _DEC(val);
    
    if (delim > 0) {// Udskriv ':' eller ' '
    myOutputFile1.print(delim);
    myOutputFile2.print(delim);
    Serial << delim;
    }
    return;
}

 void RTC_begin(){ // Opsætning af RTC modulet 
  //////////////////////////Taget fra RTC example koden
  // Activate RTC module
  
  Serial << F("RTC module activated");
  Serial << endl;
  delay(500);
  
  if (RTC.haltRTC()) {
    Serial << F("The DS1302 is stopped.  Please set time");
    Serial << F("to initialize the time and begin running.");
    Serial << endl;
  }
  if (!RTC.writeEN()) {
    Serial << F("The DS1302 is write protected. This normal.");
    Serial << endl;
  }
  
  delay(5000);
    
  //setSyncProvider() causes the Time library to synchronize with the
  //external RTC by calling RTC.get() every five minutes by default.
  setSyncProvider(RTC.get);

  Serial << F("RTC Sync");
  if (timeStatus() == timeSet)
    Serial << F(" Ok!");
  else
    Serial << F(" FAIL!");
  Serial << endl;
  
 }

 void readsensor(){ // Læser fra sensoren og udskriver dataen til brugeren
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  ha = DHTa.readHumidity();
  // Read temperature as Celsius
  ta = DHTa.readTemperature();
  // Read temperature as Fahrenheit
  fa = DHTa.readTemperature(true);
  
  // Compute heat index
  // Must send in temp in Fahrenheit!
  hia = DHTa.computeHeatIndex(fa, ha);

  // Udskriver data til Serial Monitoren
  Serial.print("Transducer A:  Humidity: "); 
  Serial.print(ha);
  Serial.print(" %       ");
  Serial.print("Temperature: "); 
  Serial.print(ta);
  Serial.print(" *C       ");
  Serial.print("Heat index: ");
  Serial.print(hia);
  Serial.println(" *F");
  
    // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  hb = DHTb.readHumidity();
  // Read temperature as Celsius
  tb = DHTb.readTemperature();
  // Read temperature as Fahrenheit
  fb = DHTb.readTemperature(true);
  
  // Compute heat index
  // Must send in temp in Fahrenheit!
  hib = DHTb.computeHeatIndex(fb, hb);

  Serial.print("Transducer B:  Humidity: "); 
  Serial.print(hb);
  Serial.print(" %       ");
  Serial.print("Temperature: "); 
  Serial.print(tb);
  Serial.print(" *C       ");
  Serial.print("Heat index: ");
  Serial.print(hib);
  Serial.println(" *F\n");
  
 }

