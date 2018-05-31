//Sawtooth
byte b = 0;
byte c = 0;
bool toggle = 1;
int test = 0;


void setup() {
  pinMode(4, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println(0);Serial.println(5); 
}

void loop() {


 // SawTooth
 //Serial.println(b++);

 //Trekant
// for(c = 0;c<255;c++){Serial.println(c);}
// for(c = 255;c>0;c--){Serial.println(c);}

/*
  //Square Wave  - Kode til opgave 1
  for(int a=0; a<=127; a++)
  {
    analogWrite(4,0);
    Serial.println(analogRead(A0));
    //delay(100);
  }
    for(int a=0; a<=127; a++)
  {
    analogWrite(4,255);
    Serial.println(analogRead(A0));
    //delay(100);
  }
  */
  


   //Trekant
// for(c = 0;c<255;c++){Serial.println(c);}
// for(c = 255;c>0;c--){Serial.println(c);}

// Trekant signal
/*
 for(c = 0;c<255;c++){
  analogWrite(4,c);
  test = analogRead(A0);
 // Serial.println((0));
  Serial.println((test));
  }
 for(c = 255;c>0;c--){
  analogWrite(4,c);
  test = analogRead(A0);
 // Serial.println((0));
  Serial.println(test);
  
  }
  */

 
 // analogWrite(4, 127);
 // Serial.println(analogRead(A0));
 // delay(1000);


 //Opgave 2:
 float angle=0;
  for(angle=0;angle<=360;angle=angle+0.1)
  {
    float sina=2*sin(angle);
    analogWrite(4,sina);
    
    Serial.println(sina+2.5);
    delay(100);
  }
  
}
