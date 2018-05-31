// opgave 4

const int tastPin = 2;
const int ledPin = 12;

int val= 0;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(tastPin, INPUT);
}

void loop(){
  val = digitalRead(tastPin);
  if (val == HIGH) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(10);
    
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}

