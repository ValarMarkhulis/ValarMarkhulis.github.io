/*Koden er skrevet af Christian Mark d. 22/10/2017
 *Koden i displayWrite() er taget fra: https://www.arduino.cc/en/Tutorial/ShftOut11 , som viste hvordan man kunne bruge shiftOut()
 *hvilket er en bedre løsning end at "shifte" værdierne og clock signalerne  ud hver for sig med digitalWrite().
 * 
 * 
 * Dokumentation:
 * http://pdf1.alldatasheet.com/datasheet-pdf/view/97528/TI/SN74HC595N.html - Datablad på Shift Register
 * https://e-radionica.com/productdata/LD3361BS.pdf - Datablad på syv-segments display
 * https://www.arduino.cc/en/Tutorial/ShftOut11 - Eksempelkode
 * 
 */

const int SRCLK = 22; 
const int SER = 23; 
const int RCLK = 24; 

const int knap0 = 3; // Pushbutton

int HexDecode(unsigned int val);
void displayWrite(unsigned int cchar, unsigned int displaySelect);
int displayedC = 0;

unsigned tal = 0;
unsigned tal_help = 0;


void setup() {
  Serial.begin(9600);

  pinMode(knap0,INPUT); 
   attachInterrupt(digitalPinToInterrupt(knap0), knap0kald , RISING);  // Interrupt sat på init 1 som er digital pin 3


  // Common Anode definitions:
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);

  // Initialisere common anodes  som LOW
  for(int i=4;i<8;i++){
      digitalWrite(i,LOW);
  }

  // Skifteregisterets styresignal definitioner:
  pinMode(SRCLK,OUTPUT);
  pinMode(SER,OUTPUT);
  pinMode(RCLK,OUTPUT);
  
  // Initialisere skifteregisterets styresignaler som LOW 
  digitalWrite(SER,LOW);
  digitalWrite(SRCLK,LOW);
  digitalWrite(RCLK,LOW);

  sei(); // Enabler global interrupt
}

void loop() {

  // Ved tryk på knap forøges variablen 'tal' som er værdien som bliver udskrevet på 4x7segement displayet. Ved tal >9999 bliver tal 0.
  if(tal < 10){
    displayWrite(tal,3);
  }else if(tal >= 10 && tal < 100){
    displayWrite(tal%10,3);
    tal_help= tal - tal%10;
    displayWrite(tal_help/10,2); 
    
  }else if(tal >=100 && tal <1000){
    displayWrite(tal%10,3);
    tal_help = ((tal - tal%10)%100)/10; // Fjerner først 'enerne' ved at trække %10 fra. Derefter ser vi om der er nogle 'tiere' og ved at dividere med 10 finder vi om hvor mange tiere der er.
    displayWrite(tal_help,2);
    displayWrite(tal/100,1); // Da en int ikke kan holde kommatal, kan vi kigge på hundrende ved at dividere med 100
    
  }else if(tal >999 && tal < 10000){
    displayWrite(tal%10,3);
    tal_help = ((tal - tal%10)%100)/10; // Fjerner først 'enerne' ved at trække %10 fra. Derefter ser vi om der er nogle 'tiere' og ved at dividere med 10 finder vi om hvor mange tiere der er.
    displayWrite(tal_help,2);    
    tal_help = (tal/100)%10;
    displayWrite(tal_help,1); // Da en int ikke kan holde kommatal, kan vi kigge på hundrende ved at dividere med 100
    displayWrite(tal/1000,0);
  }else{
    tal = 0;
  }
  

/* Skriv manuelt direkte til displayet med følgende funktion: void displayWrite(unsigned int cchar, unsigned int displaySelect)
 *  Som modtager 'cchar' som er fra '0-15' karakteren der skal skrive på displayet og paremetrene displaySelect er fra '0-3'
 *  som er hvilket af de 4 displays der skal skrive til.
displayWrite(15,0);
displayWrite(14,1);
displayWrite(11,2);
displayWrite(11,3);
*/
}

// HexDecode er et gammel kodebid som bare modtager en én char hex værdi og sender "display koden" for at lave den hex værdi på displayet
int HexDecode(unsigned int val){
  unsigned int num=0;
  switch (val)
  {
    case 0x0: num = 192;
    break;
    case 0x1: num = 249;
    break;
    case 0x2: num = 36;
    break;
    case 0x3: num = 48;
    break;
    case 0x4: num = 25;
    break;
    case 0x5: num = 18;
    break;
    case 0x6: num = 2;
    break;
    case 0x7: num = 248;
    break;
    case 0x8: num = 128;
    break;
    case 0x9: num = 144;
    break;
    case 0xA: num = 136;
    break;
    case 0xB: num = 131;
    break;
    case 0xC: num = 198;
    break;
    case 0xD: num = 161;
    break;
    case 0xE: num = 134;
    break;
    case 0xF: num = 142;
    break;
    default: num = 54; // Error besked er 3 vandrette streger
  }
  return num;
}

void displayWrite(unsigned int cchar, unsigned int displaySelect){ // Funktionen bruges til at kunne skrive addresseret til alle 4 displays
  displayedC = HexDecode(cchar);

  for(int i=4;i<8;i++){
      digitalWrite(i,LOW);
  }

  switch(displaySelect)
  {
      case 0: digitalWrite(4,HIGH);
              break;
      case 1: digitalWrite(5,HIGH);
              break;              
      case 2: digitalWrite(6,HIGH);
              break;
      case 3: digitalWrite(7,HIGH);
              break;
              
      default: for(int i=4;i<8;i++){ // Default casen slukker alle displays
                   digitalWrite(i,LOW);
                }
                                          
  }

    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(RCLK, LOW); // Registerens clock
    // shift out the bits:
    shiftOut(SER, SRCLK, MSBFIRST, displayedC);  
    //take the latch pin high so the LEDs will light up/ the output ports will have new value
    digitalWrite(RCLK, HIGH);
    // pause before next value:
    delay(6);
}

void knap0kald(){ // Knaptryk som justere hvad værdi der skrive til display
  cli();
  delayMicroseconds(30000); // Debounce pause
  tal++;

  sei();
}

