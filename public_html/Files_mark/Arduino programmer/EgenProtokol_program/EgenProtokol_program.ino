#define OUTPUT_pin 22
#define BITDELAY 20*1000
//#define debug 1
//#define debug_ 2

///////////////////
////Tidstagning////
unsigned long start = 0;
unsigned long slut = 0;
unsigned long tid = 0;
unsigned long total = 0;
double gns = 0;
unsigned long gang = 0;
///////////////////


int dataArr[5] = {'\0','\0','\0','\0','\0'}; // Char array til at mod-tage 4 cifret tal.

int incomingByte = 0;

void sendbyte(char incomingByte);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRA |= 0x01;
  PORTA |= 0x01; // Sætter 
}

void loop() {
  
  
  //if ((incomingByte = Serial.read()) != -1){
   // Serial.println(incomingByte);
    //sendbyte((char) incomingByte);
    
  //}
  sendbyte((char) 85);
  
  delay(2000);
  //incomingByte = Serial.read();

  
  // put your main code here, to run repeatedly:

}

void sendbyte(char incomingByte){
  char const temp_const = incomingByte;
  char temp = 0;

 //Serial.println(incomingByte);
  // Start bit:
  PORTA &= 0xFE;
  delayMicroseconds(BITDELAY);
  
    for(int i = 7; i >= 0 ; i--){
      
      temp = temp_const >>i;
      temp &= 0x01; //
        #ifdef debug
          Serial.println((int)temp);
        #endif
        
      if(temp){
        PORTA |= temp;
      }else{
        PORTA &= temp;
      }
        
      #ifdef debug_
        Serial.println(PORTA);
      #endif
    
      delayMicroseconds(BITDELAY);
  }
  PORTA |= 0x01; // Sætter 
  #ifdef debug
    Serial.println("\n");
  #endif
   return;
}

/*
void sendbyte(char incomingByte){
  char const temp_const = incomingByte;
  char temp = 0;
 
  // Start bit:
  PORTA &= 0xFE;
  delayMicroseconds(BITDELAY);

   
  // Første bit transmitted MSB
    #ifdef debug
        Serial.print("int sent: ");
        Serial.println((int)temp_const);
    #endif
  temp = temp_const >>7;
  temp &= 0x01; //
    #ifdef debug
      Serial.println((int)temp);
    #endif
    PORTA |= temp;
    #ifdef debug_
      Serial.println(PORTA);
    #endif

  delayMicroseconds(BITDELAY);

  // Anden bit transmitted MSB
  temp = temp_const >>6;
  temp &= 0x01; //
  #ifdef debug
    Serial.println((int)temp);
  #endif
  PORTA |= temp;
  #ifdef debug_
    Serial.println(PORTA);
  #endif
  
  delayMicroseconds(BITDELAY);

  // Trejde bit transmitted MSB
  temp = temp_const >>5;
  temp &= 0x01; //
  #ifdef debug
    Serial.println((int)temp);
  #endif
  PORTA |= temp;
  #ifdef debug_
    Serial.println(PORTA);
  #endif  
  
  delayMicroseconds(BITDELAY);

  // Fjerde bit transmitted MSB
  temp = temp_const >>4;
  temp &= 0x01; //
  PORTA |= temp;
  #ifdef debug
    Serial.println((int)temp);
  #endif
  PORTA |= temp;
  #ifdef debug_
    Serial.println(PORTA);
  #endif
  delayMicroseconds(BITDELAY);

  // Femte bit transmitted MSB
  temp = temp_const >>3;
  temp &= 0x01; //
   #ifdef debug
    Serial.println((int)temp);
  #endif
  PORTA |= temp;
  #ifdef debug_
    Serial.println(PORTA);
  #endif
  
  delayMicroseconds(BITDELAY);

  // Sjette bit transmitted MSB
  temp = temp_const >>2;
  temp &= 0x01; //
    #ifdef debug
      Serial.println((int)temp);
    #endif
   PORTA |= temp;
    #ifdef debug_
      Serial.println(PORTA);
    #endif
  
  delayMicroseconds(BITDELAY);

  // Syv bit transmitted MSB
  temp = temp_const >>1;
  temp &= 0x01; //
    #ifdef debug
      Serial.println((int)temp);
    #endif
   PORTA |= temp;
   
    #ifdef debug_
      Serial.println(PORTA);
    #endif
  
  delayMicroseconds(BITDELAY);

  // Otte bit transmitted MSB
  temp = temp_const >>0;
  temp &= 0x01; //
  #ifdef debug
    Serial.println((int)temp);
  #endif
    PORTA |= temp;
  #ifdef debug_
    Serial.println(PORTA);
  #endif

  delayMicroseconds(BITDELAY);
 


  
}
*/


