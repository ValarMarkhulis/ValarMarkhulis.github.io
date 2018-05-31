// opgave 5

const int tastPin = 2;
const int ledPin = 12;
const int ledPin1 = 13;

int val= 0;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(tastPin, INPUT);
}

void loop(){
  val = digitalRead(tastPin);
  if (val == HIGH) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin1, HIGH);
    delay(1);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin1, LOW);
    delay(1000);
   
  }
  else {
  
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin1, HIGH);
  }
}


