/* Code written and tested by Christian Mark S164833
 * The code is written as a "setup guide"/help for using the ADC0804
 * "Analog-to-Digital Converter" with the Arduino. 
 * 
 * The hardware setup can be seen on the attached picture made in fritzing.
 * The rest of the pin-layout can be found one page 3 in this .pdf: http://www.ti.com/lit/ds/symlink/adc0802-n.pdf
 * See http://www.microcontroller-project.com/adc0804-pinout.html for a beginner friendly description of the the IC's pins
 */
#define CS 7 // Pin 1 på IC | Chip select (Active HIGH) - (This activates the chip)
#define RD 8 // Pin 2 på IC | Read (Active HIGHj) - (This will bring data from internal registers to the output pins after conversion)
#define WR 9 // PIN 3 på IC | Write (Active HIGH) - (Starts the conversion by taking in the analog value)
#define INTR 10 // Pin 5 på IC | Interupt request (Active HIGH) - (This pin automatically goes low when conversion is done by the IC)

int readD = 0; // Var to hold the INTR signal

void setup() {
  
  pinMode(WR, OUTPUT);
  digitalWrite(WR,HIGH);

  pinMode(INTR,INPUT);
  
  pinMode(RD,OUTPUT);
  
  pinMode(CS, OUTPUT);
  digitalWrite(CS,HIGH);
  
  Serial.begin(9600);

}

void loop() {      
      
      digitalWrite(CS,LOW); // Chip Select aktiv
      delay(10);
      digitalWrite(WR,LOW); // Starts the conversion by making the IC write the analog value to the internal registers   
      Serial.println("\nConversion start");
      
      readD = digitalRead(INTR); // Read input INTR
      if(readD == HIGH) // if INTR is HIGH, the IC tells us that the conversion is complete
      {
          Serial.println("Conversion complete");
          digitalWrite(RD,LOW); // Read the values from the internal registers onto the output ports

          delay(1000); // Wait 1 seconds to hold the value on the output ports (for visual representation only)
          digitalWrite(RD,HIGH); // Clear the output ports
          digitalWrite(WR,HIGH); // STOP conversion
          digitalWrite(CS,HIGH); // Chip Select deaktiveret
      }

      delay(1000);

}
