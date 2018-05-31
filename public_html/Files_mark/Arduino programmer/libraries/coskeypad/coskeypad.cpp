#include "coskeypad.h"

Keypad::Keypad(byte rowarray[4], byte colarray[4]){
  rowPtr = rowarray;
  colPtr = colarray;
  
  for (int i = 0; i<4;i++) // Opsætning
  {
    pinMode((int) rowPtr[i],INPUT); // Define ROW pins as input
    digitalWrite((int) rowPtr[i],HIGH); // Pull-up on input
    
    pinMode((int) colPtr[i],OUTPUT); // Define COLUMNS pins as output
    digitalWrite((int) colPtr[i],HIGH); // Columns are HIGH
  }
}

int Keypad::scan(){
  
  for(char i = 3; i >= 0; i--){
    digitalWrite((int)colPtr[i],LOW);

    for (char j = 0; j < 4; j++)
    {
      if (digitalRead((int) rowPtr[j]) == LOW)
      {
        delay(150);
        while(digitalRead((int) rowPtr[j]) == LOW);
        digitalWrite((int) colPtr[i],HIGH);
        return keys[i][j];
      }
      
    }
    digitalWrite((int) colPtr[i],HIGH);
  }

  return 0;
  
}

int Keypad::readkey(){
  int temp = 0;

  while(temp == 0){
    temp = scan();
  }
  return temp;
}
