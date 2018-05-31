/* Version 1.5 - Højtaler og Display implementering
 *  
 * Programmet er lavet af Christian Mark - S164833            17/09/2017
 * Programmet bruger Sketchen "toneMelody" og getKey() funktionen fra Arduino Cookbook side 149.
 * 
 * Programmet er skrevet til et 4x4 keypad, men kan umiddelbart ændres til n x m Keypad. Derudover
 * er der brugt en buzzer, en RGB LED og 1 modul 7-segmentdisplay.
 * Programmet kan modtage tastetryk fra brugeren og vise det på 7-segmentsdisplayet. Buzzeren og LED'en
 * er brugt til, at vise om en bestemt sekvens er indtastet eller ej. Sekvensen kan ændres i variablen sekvensArray[].
 * Andre melodier kan også implementeres og afspilles, men er pga. "toneMelody" koden begrænset til kun at være,
 * 4-5 sekunder langt.
 * 
 * Til udvidelse kunne man arbejde videre med en idé om noget "adgangskode"/adgangskontrol ved at
 * implementere et større/flere display moduler som kunne bruges til at indskrive ord
 * eller man kunne lave et lille huskespil udaf det. Implementere et LED Matrix
 * som skulle vise en sekvens som man så via knapperne skulle gentage.
 * 
 */
 
#include "pitches.h" // Tone bibliotek
#include <string.h> // String bibliotek bruges til funktionen memcmp

const int port = 13; // Hvilken port højtaleren er sluttet til
char sekvensArray[6] = { '1', '3', '3', '7', '#'}; // En sekvens som brugeren skal indtaste på keypaden
char inputArray[6] = {'0','0','0','0','0'}; // Array der holder styr på input fra keypaden
int pointer = 0; // Holder styr på, hvor brugeren er henne i inputArray

const int numRows = 4; // number of rows in the keypad
const int numCols = 4; // number of columns
const int debounceTime = 20; // number of milliseconds for switch to be stable

// keymap defines the character returned when the corresponding key is pressed
const char keymap[numRows][numCols] = {
  { '1', '2', '3', '4'} ,
  { '5', '6', '7', '8'} ,
  { '9', 'A', 'B', 'C'} ,
  { 'D', 'E', 'F', '#'}
};

// Definere 2 forskellige melodier ved hjælp af biblioteket "pitches.h"
int melody0[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int melody1[] = {
  NOTE_G3, NOTE_C4, NOTE_G3, 0, NOTE_B3, NOTE_C4, NOTE_A3, NOTE_G3
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// this array determines the pins used for rows and columns
const int rowPins[numRows] = { 5, 4, 3, 2 }; // Rows 0 through 3
const int colPins[numCols] = { 6, 7, 8, 9 }; // Columns 0 through 2


// 7 segment display definitioner (De er allesammen på Port L)
const int a = 49; // a PL0
const int b = 48; // b PL1
const int c = 47; // c PL2
const int d = 46; // d PL3
const int e = 45; // e PL4
const int f = 44; // f PL5
const int g = 43; // g PL6 
const int DP = 42; // DP PL7

void DisplayDecoder(char key); // Funktion prototype så funktionen kan initialiseres i setup

void setup()
{
  Serial.begin(9600);

  // Definere de 3 Analog porte brugt til at styre RGB LED'en
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  // Display Porte 
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(DP, OUTPUT);

  //DisplayDecoder Funktion
  DisplayDecoder('#');
    
  for (int row = 0; row < numRows; row++){
    pinMode(rowPins[row],INPUT); // Set row pins as input
    digitalWrite(rowPins[row],HIGH); // turn on Pull-ups
  }
  
  for (int column = 0; column < numCols; column++){
    pinMode(colPins[column],OUTPUT); // Set column pins as outputs for writing
    digitalWrite(colPins[column],HIGH); // Make all columns inactive
  }
  
}

void loop()
{

  char key = getKey();
  if( key != 0) { // if the character is not 0 then it's a valid key press
    Serial.print("You Entered:  ");
    Serial.println(key);
    DisplayDecoder(key);

    inputArray[pointer] = key; // Lægger brugerens tryk ind i 'inputArray'
    pointer++; // Incrementer pointeren
    
    if (pointer > 5){ // Hvis pointeren er 5 eller over skal pointeren resettes, da programmet ellers vil fejle da arrayet kun er 5 stort
      pointer = 0;
      for ( int i = 0; i<5; i++){ // Resetter Arrayet for god ordens skyld
          inputArray[i] = '0';
      }
    }
    
    

      if (key == '#'){ // Hvis der bliver trykket '#' eller S16 på keypaden.
        
      
        if ((memcmp(sekvensArray, inputArray, 5)) == 0){ // Hvis sekvensArray == inputArray returnere funktionen memcmp() 0, hvis der er forskel returnere den != 0
            Serial.print("Secret Password Entered\n");
            Serial.print("--------------------------\n");
            
            // RGB LED'en viser grøn farve
            analogWrite(A0, 200);
            analogWrite(A1, 0);
            analogWrite(A2, 0);
            
            // Afspiller melodi0
            playmytone(0); 
            
            for(int i = 0; i <5;i++) // Får LED'en til at blinke i 6 sekunder
            {
              if(i % 2 == 0){
                analogWrite(A0, 0);
              }else{
                analogWrite(A0, 200);      
              }
              delay(1000);
            }

            
        }else{
            Serial.print("Try again\n--------------------------\n");
            
            // RGB LED'en viser Rød farve
            analogWrite(A0, 0);
            analogWrite(A1, 200);
            analogWrite(A2, 0);
            
            // Afspiller melodi1
            playmytone(1);
            analogWrite(A1, 0);
  
        }
        pointer = 0; // Reset pointeren
        for ( int i = 0; i<5; i++){
            inputArray[i] = '0';
        }
      }
  }
}

// returns with the key pressed, or 0 if no key is pressed
char getKey(){
  char key = 0; // 0 indicates no key pressed
  
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

void playmytone(int nummer){ //Funktion taget fra sketchen "toneMelody"
   // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    int noteDuration = 1000 / noteDurations[thisNote]; //the duration of the tone in milliseconds

    if(nummer == 0){
      tone(port, melody0[thisNote], noteDuration); // Spiller melody0
    }else if(nummer == 1){
      tone(port, melody1[thisNote], noteDuration);// Spiller melody1
    }
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(port);
  }
  return 0;
}

void DisplayDecoder(char key){

  switch(key){
    case '1': PORTL = 0b00000110; // 1
              break;
    case '2': PORTL = 0b01011011; // 2
              break;
    case '3': PORTL = 0b01001111; // 3
              break;   
    case '4': PORTL = 0b01100110; // 4
              break;
    case '5': PORTL = 0b01101101; // 5
              break;
    case '6': PORTL = 0b01111101; // 6
              break;     
    case '7': PORTL = 0b00000111; // 7
              break;
    case '8': PORTL = 0b01111111; // 8
              break;
    case '9': PORTL = 0b01101111; // 9
              break;   
    case 'A': PORTL = 0b01110111; // A
              break;
    case 'B': PORTL = 0b01111100; // b
              break;
    case 'C': PORTL = 0b00111001; // C
              break;  
    case 'D': PORTL = 0b01011110; // d
              break;
    case 'E': PORTL = 0b01111001; // E
              break;
    case 'F': PORTL = 0b01110001; // F
              break;                               
    default: PORTL = 0b10000000; // Prik for #
   
 //PORTL = 0b00111111; // 0
 //PORTL = 0b00000110; // 1
 //PORTL = 0b01011011; // 2
 //PORTL = 0b01001111; // 3
 //PORTL = 0b01100110; // 4
 //PORTL = 0b01101101; // 5
 //PORTL = 0b01111101; // 6
 //PORTL = 0b00000111; // 7
 //PORTL = 0b01111111; // 8
 //PORTL = 0b01101111; // 9
 //PORTL = 0b01110111; // A
 //PORTL = 0b01111100; // b
 //PORTL = 0b00111001; // C
 //PORTL = 0b01011110; // d
 //PORTL = 0b01111001; // E
 //PORTL = 0b01110001; // F
  }
return 0;
  
}
