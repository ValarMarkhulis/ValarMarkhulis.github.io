void setup() {
  pinMode(5,OUTPUT);
  //TCCR3B Initial Value is 0000 0110
  //TCCR3B = TCCR3B & 0xFF; // 490 Hz

  
  TCCR3B = TCCR3B & 0x00; //
  //TCCR3B = TCCR3B | 0x01; // 31 kHz
  //TCCR3B = TCCR3B | 0x02; // 3,92 kHz
  TCCR3B = TCCR3B | 0x05; // 30 Hz
  //TCCR3B = TCCR3B | 0x03; // 490 Hz
  //TCCR3B = TCCR3B | 0x04; // 122,5 Hz
  

}

void loop() {
  analogWrite(5, 127);

}
