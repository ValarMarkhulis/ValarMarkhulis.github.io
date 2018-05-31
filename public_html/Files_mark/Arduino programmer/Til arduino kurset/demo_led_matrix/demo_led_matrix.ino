#define debug 1

int portApins [8] = {22, 23, 24, 25, 26, 27, 28, 29}; // common cathode pins
int portLpins [8] = {42, 43, 44, 45, 46, 47, 48, 49};
void DisplayDecoder(int key);
int getKey();
long randNumber;
int j = 4;


const int numRows = 4; // number of rows in the keypad
const int numCols = 4; // number of columns

// this array determines the pins used for rows and columns
const int rowPins[numRows] = { 31, 32, 33, 34 }; // Rows 0 through 3
const int colPins[numCols] = { 35, 36, 37, 38 }; // Columns 0 through 2

//Sekvens 
int sekvensArray[4] = {0}; // Array der holder styr på sekvensen
int inputArray[20] = {0}; // Array der holder styr på input fra keypaden
int pointer = 0; // Holder styr på, hvor brugeren er henne i inputArray


const int debounceTime = 20; // number of milliseconds for switch to be stable

const int keymap[numRows][numCols] = {
  { 0, 1, 2, 3 } ,
  { 4, 5, 6, 7 } ,
  { 8, 9, 10, 11} ,
  { 12, 13, 14, 15}
};

int state = 1;


void setup() {
  randomSeed(analogRead(1)); // Taget fra arduinos dokumentation af randomseed(seed) funktion
  Serial.begin(9600); // Serial begin
  
  for (int i = 0; i < 8; i++) { // for loop is used to configure common cathodes
    pinMode(portApins[i], OUTPUT);
    digitalWrite(portApins[i], HIGH);
    pinMode(portLpins[i], OUTPUT);
    digitalWrite(portLpins[i], LOW);
  }

  for (int row = 0; row < numRows; row++){
    pinMode(rowPins[row],INPUT); // Set row pins as input
    digitalWrite(rowPins[row],HIGH); // turn on Pull-ups
  }
  
  for (int column = 0; column < numCols; column++){
    pinMode(colPins[column],OUTPUT); // Set column pins as outputs for writing
    digitalWrite(colPins[column],HIGH); // Make all columns inactive
  }

}

void loop() {

  switch(state){

      case 1:  
              for(int i = 0;i < j;i++){
                  randNumber = random(0, 16); // 16 er max. Men funktionen returner mellem min og max-1
                  Serial.print("Random nummer: ");
                  Serial.println(randNumber);
                  DisplayDecoder(randNumber);
                  delay(1000);
                  DisplayDecoder(17);
                  delay(800);
                  sekvensArray[i] = (long) randNumber;
              }
              
              if(debug){ 
                Serial.println("De tal der ligger i sekvensararry:");
                for(int i = 0;i < j;i++){
                  if(sekvensArray[i] >= 0 || sekvensArray[i] <16){
                    Serial.println(sekvensArray[i]);
                  } 
                }
              }
            
              state = 2;
              break;
    
      case 2:
              int key = getKey();
              if( key != 20) { // if the character is not 20 then it's a valid key press
                Serial.print("You Entered:  ");
                Serial.println(key);
                
                
                
                if (pointer > j){ // Hvis pointeren er 4 eller over skal pointeren resettes, da programmet ellers vil fejle da arrayet kun er 4 stort
                  pointer = 0;
                  for ( int i = 0; i<j; i++){ // Resetter Arrayet for god ordens skyld
                      inputArray[i] = 0;
                  }
                }

                inputArray[pointer] = key; // Lægger brugerens tryk ind i 'inputArray'
                pointer++; // Incrementer pointeren
                
                
              if(pointer==j){
                if ((memcmp(sekvensArray, inputArray, j)) == 0){ // Hvis sekvensArray == inputArray returnere funktionen memcmp() 0, hvis der er forskel returnere den != 0
                        Serial.println("Secret Password Entered\n");
                        state = 1;
                         j++;
                        int inputArray[j] = {0};
                        int sekvensArray[j]= {0};
                        if(memcmp(inputArray, sekvensArray, j) == 0){
                          Serial.println("Hej");
                          Serial.println(j);
                        }
            
                        
                    }else{
                        Serial.print("Try again\n--------------------------\n");
                        Serial.println("De tal der ligger i sekvensararry:");
                        for(int i = 0;i < j;i++){
                             if(sekvensArray[i] >= 0 || sekvensArray[i] <16){
                             Serial.println(sekvensArray[i]);
                              } 
                        }
                                Serial.println("De tal der ligger i Inputararry:");
                                   for(int i = 0;i < j;i++){
                            
                             Serial.println(inputArray[i]);
                              
                        }
              
                    }
                    pointer = 0; // Reset pointeren
                    for ( int i = 0; i<j; i++){
                        inputArray[i] = 0;
                    }
              }
                    
                  
                
                  
                } break;
    }
}
void DisplayDecoder(int key){

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
