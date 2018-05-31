/* Version 1.2 - Tyveri Alarm
 * Kode lavet af Sebastian Fohlmann s140498 og Christian Mark s164833
 * Programmet bruger getKey() funktionen fra Arduino Cookbook side 149.
 *
 * Programmet er skrevet til et 4x4 keypad, men kan umiddelbart ændres til n x m
 * Keypad. Derudover er der brugt en buzzer, en RGB LED, en PIR sensor og en tiltsensor
 * Programmet og medfølgende HW er en tyverialarm med følgende features:
 * PIR bevægelsessensor som kan opfange bevægelse, en tiltsensor som kan mærke om
 * PIR sensoren bliver rykket rundt. Når enten tilte sensoren eller PIR sensoren
 * opfanger noget, vil en alarm tone fra Buzzeren lyder, lyset vil blive rødt og
 * keypaden kan bruges til at slå alarmen fra igen. Dette sker ved indtastning af koden
 * '1 2 3 4'
 */

const int numRows = 4; // number of rows in the keypad
const int numCols = 4; // number of columns
const int debounceTime = 40; // number of milliseconds for switch to be stable

// RGB globale variabler til pin numre
const int blue = 30;
const int green = 29;
const int red = 28;

// keymap defines the character returned when the corresponding key is pressed
const char keymap[numRows][numCols] = {
{ '1', '2', '3','4'} ,
{ '5', '6', '7', '8'} ,
{ '9', 'a', 'b','c' } ,
{ 'd', 'e', 'f', '#'}
};

// this array determines the pins used for rows and columns
const int rowPins[numRows] = { 8 , 7, 6, 5 }; // række fra 1 til 4 på hvilken pin de sidder
const int colPins[numCols] = { 9, 10, 11, 12 }; // søjler fra 1 til 4

// Variabler der holder "kodeordet", som brugeren skal indtaste 
// i rigtigt rækkefølge for at slukke alarmen
char var0_rig = '1';
char var1_rig = '2';
char var2_rig = '3';
char var3_rig = '4';

char var0,var1,var2,var3 = '0'; // Brugerinput fra keypad bliver gemt i disse var

// Tælle var
int i = 1;
int state = 1;
int toggle = 0;

// variable til input fra til switchen
const int tiltSwitch = 23;
const int speakerPin = 31;
const int sensorPin = 22;

void setup()
{
      // Her sætter jeg mine pins forbundet til RGB til output
      pinMode(blue, OUTPUT);
      pinMode(red, OUTPUT);
      pinMode(green, OUTPUT);

      pinMode(speakerPin, OUTPUT); // Sætter speakerPin til output
      digitalWrite(tiltSwitch,HIGH); // Sætter pull-up på tiltSwitch inputtet
      
      
      Serial.begin(9600);

      // Opsætning af keypad, fra bogen
      for (int row = 0; row < numRows; row++)
      {
        pinMode(rowPins[row],INPUT); // Set row pins as input
        digitalWrite(rowPins[row],HIGH); // turn on Pull-ups
      }
      
      for (int column = 0; column < numCols; column++)
      {
        pinMode(colPins[column],OUTPUT); // Set column pins as outputs // for writing
        digitalWrite(colPins[column],HIGH); // Make all columns inactive
      }

}
void loop()
{
  // Vi har valgt at implementere en tilstandsmaskine med 2 tilstande: "Alarm inaktiv" (1) og "Alarm Aktiv" (2).
  switch (state){
   
    case 1 : // Case 1: Alarm inaktiv
              // Sætter RGB'en til at lyse blåt
              digitalWrite(blue, HIGH); 
              digitalWrite(red, LOW);
              digitalWrite(green, LOW);
              
              if (digitalRead(tiltSwitch) == LOW || digitalRead(sensorPin)== HIGH){ // Aflæser sensorene
                 delay(20); // Debounce
                   while (digitalRead(tiltSwitch) == LOW || digitalRead(sensorPin)== HIGH) // Debounce
                    {
                      digitalWrite(blue, LOW);
                      digitalWrite(red, HIGH);
                      digitalWrite(green, LOW);
                      digitalWrite(speakerPin, HIGH);
                      
                      if(toggle == 1){
                        Serial.print("Skifter til state 2\n");
                        toggle = 2;
                      }
                      
                      state = 2;
                    }

              }else{ // Hvis sensorene ikke opfanger noget
                state = 1;
                      if(toggle == 0){
                        Serial.print("Er i state 1\n");;
                        toggle = 1;
                      }
                
                
              }
             break;

    
    case 2 : // Case 2: Alarm aktiv
    
                if(toggle == 2){
                       digitalWrite(green, LOW);
                       digitalWrite(blue, LOW);
                       digitalWrite(red, HIGH);
                       Serial.print("Er i state 2\n");
                       toggle = 0;
                 }

              // Sætter speakeren til at afspille lyde
              digitalWrite(speakerPin, HIGH);
              delay(10);
              digitalWrite(speakerPin, LOW);
              delay(10);
              

              
              char key = getKey(); // Gemmer tastetryk fra keypad
              if( key != 0) { // if the character is not 0 then // it's a valid key press
                Serial.print("Got key ");
                Serial.println(key);
                
                // Gemmer keypadtryk sekvensen
                if(i==1){
                  var0 = key;
                  i++;
                }else if(i==2){
                  var1 = key;
                  i++; 
                }else if(i==3){
                  var2 = key;
                  i++;
                }else if(i==4){
                  var3= key;
                  i=1;
                }
              
              if(i == 1){
                      // Kontrollere om det brugeren har indtastet er den rigtige kode
                      if(var1 == var1_rig && var2 == var2_rig && var3 == var3_rig && var0 == var0_rig){          
                        Serial.println("Koden var rigtigt");
                        digitalWrite(red, LOW);
                        digitalWrite(green, HIGH);
                        delay(1500);
                        state = 1; // Går tilbage til state 1
                        
                      }else{
                        Serial.println("Koden var forkert");
                        digitalWrite(green, LOW);
                        digitalWrite(red, HIGH);
                      }
                
                    }
                
                }
                break;
  }


}


// returns with the key pressed, or 0 if no key is pressed
char getKey()
{
  char key = 0; // 0 indicates no key pressed
  for(int column = 0; column < numCols; column++)
{
digitalWrite(colPins[column],LOW); // Activate the current column.
for(int row = 0; row < numRows; row++) // Scan all rows for
// a key press.
{
if(digitalRead(rowPins[row]) == LOW) // Is a key pressed?
  {
    delay(debounceTime); // debounce
    while(digitalRead(rowPins[row]) == LOW);
    {
    } 
    // wait for key to be released
    
    key = keymap[row][column]; // Remember which key
    // was pressed.
  }
}
digitalWrite(colPins[column],HIGH); // De-activate the current column.
}
return key; // returns the key pressed or 0 if none
}
