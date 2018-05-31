void setup() {
  pinMode(A1, INPUT);
  pinMode(A4, INPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  analogWrite(5, analogRead(A1));
  
  Serial.println(analogRead(A4));

}
