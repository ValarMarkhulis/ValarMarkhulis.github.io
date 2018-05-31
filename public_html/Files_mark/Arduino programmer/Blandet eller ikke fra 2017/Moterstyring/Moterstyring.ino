const int motor1A =13;
const int motor1B =12;
const int motor2A =11;
const int motor2B =10;

void setup(){
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
 // pinMode(sensor, INPUT);
}

void loop()  {
  frem();
  delay(3000);

  
}

// moter1 er dem der sidder venstre! moter2 er den til højre. B er frem A er tilbage

void frem()  {
 digitalWrite(motor1A,LOW); 
  digitalWrite(motor1B,HIGH);
   digitalWrite(motor2A,LOW);
    digitalWrite(motor2B,HIGH);
}
  
