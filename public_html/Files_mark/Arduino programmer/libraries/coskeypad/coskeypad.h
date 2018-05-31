#ifndef COSKEYPAD_H
#define COSKEYPAD_H

#include "Arduino.h"

class Keypad{
  public:
  int scan();
  int readkey();
  Keypad(byte rowarray[4], byte colarray[4]);
  
  private:
  byte *rowPtr = NULL;
  byte *colPtr = NULL;
  //byte rowPins[4] = {0};
  //byte colPins[4] = {0};
  //int keys[4][4] = {{1,2,3,4},  {5,6,7,8},  {9,10,11,12},  {13,14,15,16}};
  int keys[4][4] = {{4,3,2,1},
  {8,7,6,5},
  {12,11,10,9},
  {16,15,14,13}};
};



#endif
