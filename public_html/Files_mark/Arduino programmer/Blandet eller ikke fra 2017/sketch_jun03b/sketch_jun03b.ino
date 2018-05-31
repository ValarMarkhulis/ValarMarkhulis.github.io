/*
  AnalogReadSerial
 Reads an analog input on pinA0, prints the result to the serial monitor 
 
 This example code is in the public domain: 
 http://arduino.cc/en/Tutorial/AnalogReadSerial
 */

//Variabler
int sensorValue;

void setup() {
   Serial.begin(9600);    //baud sættes til 9600
   delay(500);
}

void loop() {
    sensorValue = analogRead(A0);  
   Serial.println(sensorValue, DEC); // sensorValue  sendes  til monitor som decimalværdi
}

