/* Version 2.0 - Huske spil + læs analog vand sensor
 *
 * Kode lavet af Sebastian Fohlmann s140498 og Christian Mark s164833
 * Programmet bruger getKey() funktionen fra Arduino Cookbook side 149.
 * Programmet bruger random() funktionen indbygget i arduino til, at 
 * få et genereret en række tilfældige tal.
 * Programmet bruger også "LiquidCrystal" biblioteket, som hjælper med 
 * at betjene et 16x2 LCD display (Hitachi HD44780 driver), hvor den 
 * analoge sensors output bliver vist og hvilket level brugeren er på i spillet.
 *
 * Programmet bruger et 4x4 keypad, men kan umiddelbart ændres til n x m
 * keypad.
 * Programmet har et 8x8 rødt LED matrix, som bruges til at vise hvilken knapper
 * og i hvilken sekvens brugeren skal huske og indtaste via keypaden.
 * Programmet modtager input fra en "funduino water sensor", som sender et analog
 * signal til A0, som skrive på LCD displayet.
 * 
 * 
 * Programmet og medfølgende HW er skrevet som et huske spil, med tilhørende vand sensor.
 * Brugeren aflæser via et 8x8 display, en bestemt sekvens af 2x2 firkanter som brugeren
 * derefter skal gengive via keypaden. Brugeren kan via LCD displayet aflæse, hvilket level
 * brugeren er nået til. Derudover kan brugeren via LCD displayet aflæse på 1. linje 
 * den analoge værdi fra vand sensoren.
 */

#define debug 0 // Debug mode toggle 
#include<string.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

// Initialisering af 8x8 display matrixen.
int portApins [8] = {22, 23, 24, 25, 26, 27, 28, 29}; // cathode pins / Rækker
int portLpins [8] = {42, 43, 44, 45, 46, 47, 48, 49}; // anode pins / Koloner

int temp = 0; // var til at holde den nye analoge aflæsning fra sensoren
int temp_old = 1; // var til at holde den "gamle" analoge aflæsning fra sensoren

const int numRows = 4; // number of rows in the keypad
const int numCols = 4; // number of columns

// this array determines the pins used for rows and columns
const int rowPins[numRows] = { 31, 32, 33, 34 }; // Rows 0 through 3
const int colPins[numCols] = { 35, 36, 37, 38 }; // Columns 0 through 2

//Variabler til huske spillet
int sekvensArray[20] = {0}; // Array der holder styr på sekvensen
int inputArray[20] = {1}; // Array der holder styr på input fra keypaden
int pointer = 0; // Holder styr på, hvor brugeren er henne i inputArray
int j = 4; // Bruges til at holde styr på, hvor stort de 2 arrays er
int state = 1; // Holder styr hvor programmet er i tilstandsmaskinen

const int debounceTime = 20; // number of milliseconds for switch to be stable

const int keymap[numRows][numCols] = {
  { 0, 1, 2, 3 } ,
  { 4, 5, 6, 7 } ,
  { 8, 9, 10, 11} ,
  { 12, 13, 14, 15}
};


// Prototyper
void DisplayDecoder(int key);
int getKey();

void setup() {
  randomSeed(analogRead(1)); // Taget fra arduinos dokumentation af randomseed(seed) funktion. Den læser fra A0 pin og bruger det som seed
  Serial.begin(9600); // Serial begin

   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Opsætningen af 8x8 matrix displayet. Det styres ved, at man bestemmer hvor der skal være grounded
  // og dermed, hvilke LED'er der skal have strøm løbende igennem sig.
  for (int i = 0; i < 8; i++) { 
    pinMode(portApins[i], OUTPUT);
    digitalWrite(portApins[i], HIGH);
    pinMode(portLpins[i], OUTPUT);
    digitalWrite(portLpins[i], LOW);
  }

  // Opsætning af keypad
  for (int row = 0; row < numRows; row++){
    pinMode(rowPins[row],INPUT); // Set row pins as input
    digitalWrite(rowPins[row],HIGH); // turn on Pull-ups
  }
  // Opsætning af keypad
  for (int column = 0; column < numCols; column++){
    pinMode(colPins[column],OUTPUT); // Set column pins as outputs for writing
    digitalWrite(colPins[column],HIGH); // Make all columns inactive
  }

}

void loop() {

  // Aflæser water sensoren, opdatere LCD displayet's første linje med "WS (Water Sensor) outputtet aflæst fra A0.
  temp = analogRead(A0);
  if(temp_old != temp){ //Hvis der er behov for at opdatere displayet
      lcd.setCursor(0,0); // Sætter cursoren på 0,0 på displayet
      lcd.print("WS output:");
      lcd.setCursor(10,0); // Sætter cursoren på plads 10,0 som er efter ":" karakteren
      lcd.print(temp);
      temp_old = temp;    
  }

  switch(state){ // Tilstandsmaskinen, hvor state er initialiseret til 1

      case 1: // Udfyld sekvensArray med tilfældige genereret tal.
      
              // Skriver til Serial Monitoren og LCD displayet
              lcd.setCursor(0,1);
              lcd.print("Level: ");
              lcd.setCursor(6,1);
              lcd.print(j-4); // j er initialiseret til 4 og vil blive incrementeret når brugeren gætter rigtigt.
              Serial.print("Level: ");
              Serial.println(j-4);
              
              for(int i = 0;i < j;i++){ // Loop der udfylder sekvensarray med tilfældige tal
                  randNumber = random(0, 16); // 16 er max. Men funktionen returner mellem min og max-1
                  Serial.print("Random nummer: ");
                  Serial.println(randNumber);
                  DisplayDecoder(randNumber); // Får 'randNumber' 2x2 LED'en til at lyse
                  delay(1000);
                  DisplayDecoder(17); // Får 'randNumber' til at slukke får alle LED'erne
                  delay(800);
                  sekvensArray[i] = (long) randNumber; // Lægger det tilfældigt genereret tal ind i sekvensArray 
              }
                        
              
              if(debug){ //Debug: Udskriver til Serial Monitoren, hvilke ting der ligger i sekvensarrayet
                Serial.println("De tal der ligger i sekvensarraryet:");
                for(int i = 0;i < j;i++){
                  if(sekvensArray[i] >= 0 || sekvensArray[i] <16){ // Kontrollere, at tallene er mellem 0-15
                    Serial.println(sekvensArray[i]);
                  } 
                }
              }
            
              state = 2;
              break;
    
      case 2: // Modtag input fra brugeren igennem keypad     
              
              int key = getKey(); // Modtag key fra keypad
              if( key != 20) { // if the character is not 20 then it's a valid key press
                Serial.print("You Entered:  ");
                Serial.println(key);

                inputArray[pointer] = key; // Lægger brugerens tryk ind i 'inputArray'
                pointer++; // Incrementer pointeren
                
                
                if(pointer==j){
                  if (((memcmp(sekvensArray, inputArray, j*2))) == 0){ // Hvis sekvensArray == inputArray returnere funktionen memcmp() 0, hvis der er forskel returnere den != 0
                          Serial.println("Secret Password Entered");
                          Serial.println("--------------------------");
                          
                          state = 1; // Programmet skal hoppe videre til næste lvl og derfor udfylde 'sekvensArray' med nye tilfældige tal
                          j++; // Incrementer j, som holder styr på størrelsen af array'sne
                      }else{
                          Serial.print("Try again\n--------------------------\n");
                          
                          if(debug){ // Debug: Udskriver hvad programmet har liggende i 'sekvensArray' og 'inputArray'
                              Serial.println("De tal der ligger i sekvensarray:");
                              for(int i = 0;i < j;i++){
                                   if(sekvensArray[i] >= 0 || sekvensArray[i] <16){
                                         Serial.println(sekvensArray[i]);
                                    } 
                              }
                          
                              Serial.println("De tal der ligger i Inputararry:");
                              for(int i = 0;i < j;i++){
                                  Serial.println(inputArray[i]);
                              }
                          } // Slut debug
                      } // Slut else
                      
                      for ( int i = 0; i<j; i++){ // Resetter inputArray
                          inputArray[i] = 0;
                      }
                      pointer = 0; // Reset pointeren
                      
                } // Slut if(pointer==j)
                    
 
            } // Slut if(key != 20)
            break;
    }// Slut switch
}

void DisplayDecoder(int key){ // Sørger for at skrive til 8x8 displayet
//PORTL = Styrer kolonner
//PORTA = Styrer rækkerne

/* PORTL er initialiseret som HIGH og PORTA er som LOW. 
 * Den måde de styres på er, at man skriver HIGH
 * på den række man gerne vil have til at lyse.
 * For at få en enkelt led på den angivede række
 * til at lyse, sættes kolononen man gerne vil 
 * have til at lyse til LOW; da der så kan 
 * løbe strøm igennem LED'erne og få
 * dem til at lyse. 
 */

  switch(key){    
    case 0: PORTL = 3; PORTA = 63;
              break;
    case 1: PORTL = 12;  PORTA = 63;
              break;
    case 2: PORTL = 48;  PORTA = 63;
              break;   
    case 3: PORTL = 192;  PORTA = 63;
              break;
    case 4: PORTL = 3; PORTA = 207;
              break;
    case 5: PORTL = 12; PORTA = 207;
              break;     
    case 6: PORTL = 48; PORTA = 207;
              break;
    case 7: PORTL = 192; PORTA = 207;
              break;
    case 8: PORTL = 3; PORTA = 243;
              break;
    case 9: PORTL = 12; PORTA = 243;
              break;   
    case 10: PORTL = 48; PORTA = 243;
              break;
    case 11: PORTL = 192; PORTA = 243;
              break;
    case 12: PORTL = 3,PORTA = 252;
              break;  
    case 13: PORTL = 12,PORTA = 252;
              break;
    case 14: PORTL = 48,PORTA = 252;
              break;
    case 15: PORTL = 192,PORTA = 252;
              break;                               
    default: PORTL = 0,PORTA = 255;
   
  }
//Her er de (2x2)x4 = 16 firkanter der ønskes oplyst samlet som små 2x2 firkanter
//firkant 0   PORTL = 3; PORTA = 63;
//firkant 1   PORTL = 12;  PORTA = 63;
//firkant 2   PORTL = 48;  PORTA = 63;
//firkant 3   PORTL = 192;  PORTA = 63;

//firkant 4   PORTL = 3; PORTA = 207;
//firkant 5   PORTL = 12;  PORTA = 207;
//firkant 6   PORTL = 48;  PORTA = 207;
//firkant 7   PORTL = 192;  PORTA = 207;

//firkant 8    PORTL = 3; PORTA = 243;
//firkant 9   PORTL = 12;  PORTA = 243;
//firkant 10   PORTL = 48;  PORTA = 243;
//firkant 11   PORTL = 192;  PORTA = 243;

//firkant 12    PORTL = 3,PORTA = 252;
//firkant 13   PORTL = 12;  PORTA = 252;
//firkant 14   PORTL = 48;  PORTA = 252;
//firkant 15   PORTL = 192;  PORTA = 252;

}

// returns with the key pressed, or 0 if no key is pressed
int getKey(){
  int key = 20; // 20 indicates no key pressed
  
  for(int column = 0; column < numCols; column++)
  {
    digitalWrite(colPins[column],LOW); // Activate the current column.
    for(int row = 0; row < numRows; row++) // Scan all rows for a key press.
    {
      if(digitalRead(rowPins[row]) == LOW) // Is a key pressed?
      {
        delay(debounceTime); // debounce
        while(digitalRead(rowPins[row]) == LOW); // wait for key to be released
        key = keymap[row][column]; // Remember which key was pressed.
      }
    }
    digitalWrite(colPins[column],HIGH); // De-activate the current column.
   }
  return key; // returns the key pressed or 0 if none
}
