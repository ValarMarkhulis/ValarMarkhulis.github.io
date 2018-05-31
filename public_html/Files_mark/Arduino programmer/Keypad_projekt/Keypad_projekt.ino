#include <avr/io.h>
#include <coskeypad.h>

const byte rowPins[4] = { 23, 25, 27, 29 };
const byte colPins[4] = { 31, 33, 35, 37 };

int sekvensarray[4] = {1,2,3,4};
int inputarray[4] = {0,0,0,0};

Keypad Keypad1(rowPins, colPins); // Opret et "Keypad" objekt Keypad1

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  /*
  // Bruger readkey() til, at vise at programmet stopper og kører først
  // når der er blevet trykket på en knap og sluppet
  Serial.println("Hello World");
  int test = Keypad1.readkey();
  Serial.println(test);
  */

  /*
  // Kodetjek() kontrollere for, om den rigtigte kode er indtastet før den går videre.
  while(kodetjek() != 1){
    ;
  }
  Serial.println("hej, resten af programmet kører.");

  */
 
  
  // Bruger scan() til, at vise at resten af programmet (Hello world udskrivningen) kører
  // mens der scannes for et knap tryk
  Serial.println("Hello World");
  int test = Keypad1.scan();
  
  if(test != 0){
    Serial.println(test);
  }
  
}

int kodetjek(){
  
  for(int j = 0; j < 4;j++){
    inputarray[j] = Keypad1.readkey();
    Serial.print(inputarray[j]);
    if(j!= 3){
      Serial.print("-");
    }
  }
   
  for(int j = 0; j < 4;j++){
    if(inputarray[j] != sekvensarray[j]){
      Serial.println(" - Forkert kode");
      return 0;
    }
  }
     
  Serial.println(" - Korrekt kode");
  return 1;  
}
