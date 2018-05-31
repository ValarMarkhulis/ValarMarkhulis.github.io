const int a = 49; // a PL0
const int b = 48; // b PL1
const int c = 47; // c PL2
const int d = 46; // d PL3
const int e = 45; // e PL4
const int f = 44; // f PL5
const int g = 43; // g PL6 
const int h = 42; // h PL7

const int rowPins[8] = { a , b, c, d, e, f, g, h };

void setup() {
  Serial.begin(9600);
  for(int i = 0;i<9;i++){
  pinMode(i,OUTPUT);
  }


}

void loop() {

    for(int i = 0; i <260;i+=5){
    delay(1000);
    PORTL = i;
    Serial.println(i);
    }
  delay(10000);
}
