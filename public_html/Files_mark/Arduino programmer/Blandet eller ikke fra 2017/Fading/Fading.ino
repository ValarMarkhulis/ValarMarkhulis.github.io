/*
 Fading

 This example shows how to fade an LED using the analogWrite() function.

 The circuit:
 * LED attached from digital pin 9 to ground.

 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe

 http://arduino.cc/en/Tutorial/Fading

 This example code is in the public domain.

 */


int ledPin = 9;    // LED connected to digital pin 9
int knap = 7;

void setup() {
  Serial.begin(9600);
  pinMode(knap, INPUT);
}

void loop() {
  int knaptryk = digitalRead(knap); // Læs værdien på pin.
  digitalWrite(ledPin, LOW);
  
  if(knaptryk != 0) {
    Serial.println("Knappen trykkes");
  // fade in from min to max in increments of 5 points:
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue++) {
      // sets the value (range from 0 to 255):
      analogWrite(ledPin, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(50);
    }
          Serial.println("Lyset slukker om 2 sekunder:");
          delay(2000);
          analogWrite(ledPin, 0);

    }
  
}


