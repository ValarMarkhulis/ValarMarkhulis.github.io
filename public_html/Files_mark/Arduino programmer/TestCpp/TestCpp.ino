#include "Arduino.h"

class Keypad{
	public:
	int scan();
	// int read();
	Keypad(byte rowarray[4], byte colarray[4]);
	
	private:
	byte *rowPtr = NULL;
	byte *colPtr = NULL;
	//byte rowPins[4] = {0};
	//byte colPins[4] = {0};
	int keys[4][4] = {{1,2,3,4},
	{5,6,7,8},
	{9,10,11,12},
	{13,14,15,16}};
};

const byte rowPins[4] = { 31, 33, 35, 37 };
const byte colPins[4] = { 23, 25, 27, 29 };

Keypad Keypad1(rowPins, colPins);
	

void setup()
{
	
	Serial.begin(9600);
	/* add setup code here */
	pinMode(13,OUTPUT); // debug
	
		for (int i = 0; i<4;i++) // Opsætning
	{
		pinMode((int) rowPtr[i],INPUT); // Define ROW pins as input
		digitalWrite((int) rowPtr[i],HIGH); // Pull-up on input
		
		pinMode((int) 23,OUTPUT); // Define COLUMNS pins as output
		digitalWrite((int) 23,HIGH); // Columns are HIGH
	}

	

}

void loop()
{
	
	//Serial.println("Hej Verden!");
	Keypad1.scan();
	//Serial.println((int)Keypad1.scan());
	/* add main program code here */

}

Keypad::Keypad(byte rowarray[4], byte colarray[4]){
	rowPtr = rowarray;
	colPtr = colarray;
	

}

int Keypad::scan(){
	
	
	for(size_t i = 3; i >= 0; i--){
		digitalWrite((int)colPtr[i],LOW);

		for (size_t j = 0; j < 4; j++)
		{
			if (digitalRead(rowPtr[j]) == LOW)
			{
				delay(50);
				
				return keys[i][j];
			}
			
		}
		digitalWrite(colPtr[i],HIGH);
	}
	
	
}