unsigned int test = 0;
unsigned int test1 = 0;
unsigned long duration = 0;
unsigned long duration1 = 0;
unsigned long duration2 = 0;

unsigned long high_time;
unsigned long low_time;
float time_period;
float frequency = 0;
float frequency_old = 1;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  //Serial.println(0);Serial.println(5); 
}

void loop() {
  
  test = analogRead(A0);
  Serial.println(test);
  /*Serial.println("Frequency Meter");
  while(1){

  high_time=pulseIn(A0,HIGH);
  low_time=pulseIn(A0,LOW);
  
  time_period=high_time+low_time;
  time_period=time_period/1000;
  frequency=1000/ (long) time_period;

  if(frequency_old != frequency && frequency >0){
  Serial.print(frequency);
  Serial.println(" Hz");
  frequency_old = frequency;
  }
  
  }
  */

}
