// Libraries to drive the SD Card. ******************************
#include <SD.h>

/* The SD card communicates over pins for the Arduino Mega:
 MOSI - Pin 51
 MISO - Pin 50
 Clock - Pin 52
 Chip Select - pin 53
 
 Yellow LED goes on during a read operation.
 Red LED goes on during a write operation.
 
 Use '(char *)filename.c_str()' to pass a string variable to concatinate the file name.
 Use 'filename.c_str()' to pass a variable file name to the FILE.open(fn,option) command.
 Make sure to disconnect the power to the SD card shield prior to removing
 the chip.
 */


int baudrate = 9600; //set the baud rate.
int buzzerPin = 2; //set buzzer pin.
const int redLED = 8; //set yellow LED.
const int yellowLED = 7; //set red LED.
int yellowLEDstate = LOW; //save the red LED state.
int redLEDstate = LOW; //save the yellow LED state.
String filename; //Store file name.
boolean isDirectory = false; //Set for directory flag
boolean isRemoveDirectory = false; //Set delete (file/director) flag.
boolean isRemoveFile = false;
boolean isChangeDirectory = false; //Set change directory flag.
boolean isReadFromFile = false;
boolean isWriteToFile = false;
String root = "/";
String tvRemoteOption;
String dataInput;
int var; //Case statement variable.
int countReadFromFile = 0; //Test.
int countWriteToFile =0;//Test.
int countRemoveDirectory =0;//Test.
int countRemoveFile =0;//Test.
int countMakeDirectory = 0;//Test.
int countMakeFile = 0;//Test.
int countChangeDirectory = 0;//Test.
int countListDirectory = 0;//Test.
int countMultipleFilesOpen = 0; //Test.
int loopCounter = 0;//Test.
int rootCounter = 1;//test.
int CSpin = 53; //Chip Select pin.
File myFile, myFile2, myFile3,myFile4, //Define files to open.
myFile5,myFile6,myFile7,myFile8,myFile9,
myFile10,myFile11,myFile12,myFile13,myFile14,
myFile15,myFile16,myFile17,myFile18,myFile19,
myFile20;
File myFiles[20] = {myFile, myFile2, myFile3,myFile4, //Create an array of files to open.
myFile5,myFile6,myFile7,myFile8,myFile9,
myFile10,myFile11,myFile12,myFile13,myFile14,
myFile15,myFile16,myFile17,myFile18,myFile19,
myFile20};





//setup *************************************************************************
void setup()
{
  Serial.begin(baudrate); // Start serial communication.
 // pinMode(10,OUTPUT);//Required by the SD library?
  pinMode(CSpin,OUTPUT);//Required by the SD library?

  
  pinMode(redLED, OUTPUT); //Setup yellow LED.
  pinMode(yellowLED, OUTPUT);  //Setup red LED.
  pinMode(buzzerPin, OUTPUT); //Setup buzzer.
  
  Serial.println(F("************************************************************"));
  Serial.println(F("************************************************************"));
  Serial.println();
  
  Serial.println(F("Initialize the SD card..."));
  delay(2000);
  
  if (!SD.begin(CSpin)){  //Test for initialized.
    ErrorHandler(9);
  }//End if.
  

  Serial.println(F("Initialization Complete."));
  tone(buzzerPin, 440, 10);
  delay(50);
  noTone(buzzerPin); // turn off tone function.
  
  root = "/"; //Set the root directory.

  digitalWrite(yellowLED, LOW); //Initialize the LED to OFF.
  digitalWrite(redLED,LOW); //Initialize the LED to OFF.
 
 

}//End setup.


//loop *********************************************************************************
void loop()
{
loopCounter = loopCounter + 1;  //Test.

 Serial.println();
 Serial.print(F("Loop Counter--> "));
 Serial.println(loopCounter);
 
 var= random(1,10); //Test random CASE option.
 filename = ""; //test: reset filename. line 200


  var = 1; //Choose specific CASE option to test.
 
switch(var){

case 1:  
  Serial.println(F("List File..."));
  ListDirectory();//Call function.
  countListDirectory = countListDirectory + 1;
break;

case 2:
 Serial.println(F("Change Directory..."));
  ChangeDirectory(); //Call function.
  countChangeDirectory = countChangeDirectory + 1;
break;

case 3:
   Serial.println(F("Make File..."));
  MakeFile(); //Call function.
  countMakeFile = countMakeFile + 1;
break;

case 4:  
   Serial.println(F("Make Directory..."));
  MakeDirectory(); //Call function.
  countMakeDirectory = countMakeDirectory + 1;
break;

case 5:  
 Serial.println(F("Remove File..."));
  RemoveFile(); //Call function.
  countRemoveFile = countRemoveFile + 1;
break;

case 6: 
 Serial.println(F("Remove Directory..."));
  RemoveDirectory(); //Call function.
  countRemoveDirectory = countRemoveDirectory + 1;
break;

case 7:  
  Serial.println(F("Write To File..."));
  WriteToFile(); //Call function.
  countWriteToFile = countWriteToFile + 1;
break;

case 8:
 Serial.println(F("Read From File..."));
  ReadFromFile(); //Call function.
  countReadFromFile = countReadFromFile + 1;
break;

case 9:
 Serial.println(F("Multiple Files Opened..."));
 MultipleFilesOpen();
 countMultipleFilesOpen = countMultipleFilesOpen + 1;
break;


 } //End case statement.
 
 

if( loopCounter >= 1){
  
Serial.println(F("****************************************"));

Serial.print(F("Total List Directory..."));
Serial.println(countListDirectory);

Serial.print(F("Total Change Directory..."));
Serial.println(countChangeDirectory);

Serial.print(F("Total Make File..."));
Serial.println(countMakeFile);

Serial.print(F("Total Make Directory..."));
Serial.println(countMakeDirectory);

Serial.print(F("Total Remove File..."));
Serial.println(countRemoveFile);

Serial.print(F("Total Remove Directory..."));
Serial.println(countRemoveDirectory );

Serial.print(F("Total Write To File..."));
Serial.println(countWriteToFile);

Serial.print(F("Total Read From File..."));
Serial.println(countReadFromFile);

Serial.print(F("Total Multiple Files Opened..."));
Serial.println(countMultipleFilesOpen);

//ReadFromFile(); //Call function.

while(true); //Test.Stop program.
} //End if.

} //End loop.



//WriteToFile  ****************************************************
//Note: 2 files are open simultaneously.
 void WriteToFile(){
   redLEDstate = flashLED(redLED, redLEDstate); //Turn LED on.
   isWriteToFile = true;
 
   Serial.println(F("(1)Write to the file...")); //debug.
 

    
    myFiles[0] = SD.open("data1.txt", FILE_WRITE); //Open data file.
    delay(200);//Wait for file to open.
    if(!SD.exists("data1.txt")){ //Check if file exists.
    filename = "data1.txt";
    ErrorHandler(4);}
   
    myFiles[1] = SD.open("test.txt",FILE_READ); //Open input file.
    delay(200);//Wait for file to open.
    if(!SD.exists("test.txt")){ //Check if file exists.
    filename = "test.txt";
    ErrorHandler(4);}

 
 /* 
 //This code resets the data1.txt output file. **********************************
  myFiles[1] = SD.open("data1.txt", FILE_WRITE); //Select file to clear.
  delay(200);//Wait for file to open.
 // SD.remove("datalog.txt"); //Must use (char *)filename.c_str() to work.
 // delay(200); //Wait for SD card to complete.
  myFiles[1].seek(0); //reset to the beginning of the file each loop.
  for(int i = 1; i < 7; i = i + 1){
  myFiles[1].print("          "); //Initialize the file datalog.txt with 10 blanks.
 }//end for loop. 
  myFiles[1].close(); //Write the file to the SD card.
//*******************************************************************************
*/    
   
    myFiles[1].seek(10);//set input file position to be read at startup.
    for (int i = 1; i < 11; i++){ //Number of characters to read.
    dataInput += (char) myFiles[1].read(); //Read each character and concatinate to data string.
    }//end for.
    
 //   int filePosition = random( 0, myFile.size() );

    tvRemoteOption = "1POWER"; //Set veriable.

    int x = 0; //use to index the seek() option on the SD Card.


    myFiles[0].seek(x);
    myFiles[0].print(tvRemoteOption.c_str()); //Pass a variable to save to the SD Card.
  
    myFiles[0].seek(x+10);
    myFiles[0].print(dataInput); //Write data from file 2 to file 1 on SD card.
    dataInput = "";//Reset dataInput string for next use.
    
     myFiles[0].seek(x+20);
     myFiles[0].print("3CHAN UP");    
    
     myFiles[0].seek(x+30);
     myFiles[0].print("4CHAN DWN"); 
    
    delay(200);
    
    myFiles[0].close(); //Close the file and save the data.
    delay(200); //Wait for file to close.
    myFiles[1].close(); //Close the file and save the data.
    delay(200); //Wait for file to close.
    
    
  isWriteToFile = false; 
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED off.
} //end WriteToFile function.


//MultipleFilesOpen *************************************************
void MultipleFilesOpen()
 {  
     redLEDstate = flashLED(redLED, redLEDstate); //Turn LED on
   
    Serial.println(F("(1)Multiple files opened...")); //debug.
   
   
    myFiles[0] = SD.open("test.txt", FILE_WRITE); //Open data file.
    delay(200);//Wait for file to open.
 
    
//Use an array of myFiles to open and name each text file to write to. *****************   
    for( int y = 1; y < 20; y ++){
//Open the file to write to.Converts int to string.Concatinate to create the text filename.
     myFiles[y] = SD.open(("data" + String(y) + ".txt").c_str() , FILE_WRITE);
    
    } //End for.
 //************************************************************************************    
    

 
   for (int x = 1; x < 20; x++){ //Exclude test.txt at position 0.
     myFiles[0].seek( random(0,myFiles[0].size() - 100 ) );//Set read test.txt position. 
     for ( int i =1; i < 11; i++){  //Read 10 characters from test.txt.
       dataInput += (char) myFiles[0].read(); //Read the characters from test.txt.
     } //End for.
     myFiles[x].print(dataInput.c_str()); //Write to the open file.
     dataInput = ""; //Reset the input data.
   }//End For.


//   int filePosition = random( 0, myFiles[x].size() ); 
  
//    myFiles2.seek(13);//set input file offset position to be read.  
//    myFiles[0].seek(140);//set input file offset position to be read.
//    for (int i = 1; i < 11; i++){ //Number of characters to read.
  //  dataInput += (char) myFile2.read(); //Read each character and concatinate to data string.
//  dataInput += (char) myFiles[0].read(); //Read each character and concatinate to data string.
//    }//end for.
    
/*  
//Create data to write to file. **************************
    tvRemoteOption = "Multiple ("; //Set veriable.

    myFiles[1].seek(0);
    myFiles[1].print(tvRemoteOption.c_str()); //Pass a variable to save to the SD Card.
  
    myFiles[1].seek(10);
    myFiles[1].print(dataInput.c_str()); //Write data from file 2 to file 1 on SD card.
    dataInput = "";//reset dataInput.
 
    myFiles[1].seek(20);
    myFiles[1].print(")files are");    
    
     myFiles[1].seek(30);
     myFiles[1].print(" working."); 
    
    delay(200);
 ********************************************************/ 
  
  
//Close all the open files. ***************************************************** 
  for (int x = 0; x < 20; x++){  
    myFiles[x].close(); //Close the file and save the data.
    delay(200); //Wait for file to close.
  } //Close all the files.
 
//**********************************************************************************   



   
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED off.  
}//End MultipleFilesOpen.



//ReadFromFile ***************************************************
void ReadFromFile(){
  yellowLEDstate = flashLED(yellowLED, yellowLEDstate); //Turn LED on.
  isReadFromFile = true;

   Serial.println(F("(1)Read from the file...")); //debug.
 

    myFiles[0] = SD.open("data1.txt", FILE_READ);
    delay(200);//Wait for file to open.
    if(!SD.exists("data1.txt")){ //Check if file exists.
    filename = "data1.txt";
    ErrorHandler(4);}


    while (myFiles[0].available()) { //Read until EOF.
    Serial.write(myFiles[0].read()); //Read data from SD Card and write the file contents to the Serial Monitor.
    } //End while
    myFiles[0].close(); //Close the file and save the data.
    delay(200); //Wait for file to close.
 
   Serial.println(); 
   
   isReadFromFile = false;
   yellowLEDstate = flashLED(yellowLED, yellowLEDstate); //Turn LED off.

  } //end ReadFromFile function.




//ChangeDirectory ****************************************************
//The ChangeDirectory function is the only function that modifies the
//'root' variable.
//********************************************************************
void ChangeDirectory(){
  isDirectory = true; //Set directory flag.
  isChangeDirectory = true; //Debug.Set change directory flag.

  Serial.println(F("(1)Change Directory to...")); //debug.

 //   while (Serial.available() < 1) {} //Wait for serial montior input.
//    filename = Serial.readString();
 
  //generate a random file name.Remove.
  int i = random(65,90);//test.
  filename = String((char) i);//test.

 //   if(filename == "/"){   //Must enter "/" to return to root directory.
 //     root = "/";
 //   } //end if.
  if(rootCounter <= 2){
      root = root + filename.c_str() + "/"; //Add in '(char *)' ????? Build root directory.
      rootCounter = rootCounter + 1;//test.
      Serial.print(F("(2)ROOT: "));//test.
      Serial.println(root);//test.
  } //end if.
    else {
      root = "/"; //test.
      rootCounter = 1; //test.
      Serial.print(F("(2)ROOT: "));//test.
      Serial.println(root);//test.  
    } //End else.
     
  isDirectory = false; //Set directory flag.
  isChangeDirectory = false; //Debug.Set change directory flag.
  
} //End ChangeDirectory function.


//MakeDirectory **************************************************
void MakeDirectory(){
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED on.
  isDirectory = true; //This may need to be moved to the filename function.

  Serial.println(F("(1)Make the directory...")); //debug.
 
   //generate a random file name.Remove.
  int i = random(65,90);//test.
  filename = String((char) i);//test.
  Serial.print(F("(2)Directory: "));//test.
 
 
 // while (Serial.available() < 1) {} //Wait for serial montior input.
   filename = root + filename;
   Serial.println(filename);//test.

  SD.mkdir((char *)filename.c_str()); 
  delay(500); //Wait for SD card to complete.
  myFile.close(); //Write the file to the SD card.
  delay(500); //Wait for SD card to complete.
 
  isDirectory = false; //reset the directory flag.
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED off.
  
} //End MakeDirectory function.




//MakeFile  **********************************************************
 void MakeFile(){
   redLEDstate = flashLED(redLED, redLEDstate); //Turn LED on.
   Serial.println(F("(1)Make a file...")); //debug.
   
//   EnterFileName();//Check the file name.

  //generate a random file name.Remove.
  int i = random(65,90);//test.Create a chara A to Z.
  filename = root + String((char) i) + ".txt";//test.
  Serial.print(F("(2)"));//test.
  Serial.println(filename);//test.
  
   myFiles[0] = SD.open(filename.c_str(), FILE_WRITE);
   delay(200);//Wait for file to open.
   myFiles[0].close(); //Close the file and save it to the SD card.
   delay(200);//Wait for file to close.
 
   if(!SD.exists((char *)filename.c_str())){ //Check if file exists.
   ErrorHandler(3);}
 

  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED off.
  
} //end MakeFile function.



//RemoveFile ******************************************************
void RemoveFile(){
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED on.
  isDirectory = false; //reset the directory flag.
  isRemoveFile = true; //set delete flag.

   
   Serial.println(F("(1)Remove the file...")); //debug.
  
//  EnterFileName();//Check the file name.

  //generate a random file name.Remove.
  int i = random(65,90);//test.
  filename = root + String((char) i) + ".txt";//test.
  Serial.print(F("(2)"));//test.
  Serial.println(filename);//test.

 
  SD.remove((char *)filename.c_str()); //Must use (char *)filename.c_str() to work.
  delay(500); //Wait for SD card to complete.
  myFile.close(); //Write the file to the SD card.
  delay(500); //Wait for file to close.
  
  isRemoveFile = false; //set delete flag.
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED off.
  
} //End MakeDirectory function.



//RemoveDirectory **************************************************
void RemoveDirectory(){
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED on.
  isDirectory = true; //This may need to be moved to the filename function.
  isRemoveDirectory = true; //set delete flag.

  
  
  Serial.println(F("(1)Remove the directory...")); //debug.
 
 // EnterFileName();//Check the file name.
 
 
  //generate a random file name.Remove.
  int i = random(65,90);//test.
  filename = root + String((char) i);//test.
  Serial.print(F("(2)Directory: "));//test.


  Serial.println(filename);//test.
 
  
  SD.rmdir((char *)filename.c_str()); 
  delay(500); //Wait for SD card to complete.
  myFile.close(); //Write the file to the SD card.
  delay(500); //Wait for file to close.

  isDirectory = false; //reset the directory flag.
  isRemoveDirectory = false; //set delete flag.
  redLEDstate = flashLED(redLED, redLEDstate); //Turn LED off.
  
} //End RemoveDirectory function.




//ListDirectory **************************************************
void ListDirectory(){
  
    yellowLEDstate = flashLED(yellowLED, yellowLEDstate); //Turn LED on.
  
    myFile = SD.open("/"); //Root directory.
    delay(200);//Wait for myFile to open.
    PrintDirectory(myFile, 0);
    myFile.close();//Close the myFile.
    delay(200);//Wait for file to close.
    
    yellowLEDstate = flashLED(yellowLED, yellowLEDstate); //Turn LED off.
    
}//end ListDirectory function.


// PrintDirectory **************************************************
// This code is from the arduino library SD Listfiles ( https://www.arduino.cc/en/Tutorial/listfiles ).
// The code does not work correctly. The last entrys in a subdirectory does not display.

void PrintDirectory(File dir, int numTabs) {
  dir.seek(0); // Add seek!!!

  while (true) {

    File entry =  dir.openNextFile();
     if (! entry) {
      dir.rewindDirectory();
      break; 
    } //End if. 
    
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    } //End for.
   
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      PrintDirectory(entry, numTabs + 1);
    } //End if. 
    
    else 
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    } //End else
    
    entry.close(); //Write the file to the disk.
    
  } //End while.
  
} //End PrintDirectory function.

//flashLED **********************************************************
int flashLED(int ledPin,int ledState){


    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } //end if.
   
    else {
      ledState = LOW;
    } //end else.

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
//  } //end if.

return(ledState); //Return the LED state.

} //end flashLED function.



//ErrorHandler ***************************************************
//Use this function to trap and display error messages. If an error
//occurs stop all processing.
void ErrorHandler(int errorCode){

 
switch(errorCode){

case 1:  
  Serial.print(F("ERROR[1]: "));
  Serial.print(filename);
  Serial.println(F(" file not found."));
  tone(buzzerPin, 494, 100);  //Debug.
  delay(200); //Debug.
  noTone(buzzerPin);//Debug. Turn off tone function.
  while(true); //Stop running.
break;

case 2:
    Serial.print(F("ERROR[2]: directory "));
    Serial.print(filename);
    Serial.println(F(" does not exist.")); //Error message.
    tone(buzzerPin, 494, 100);  //Debug.
    delay(200); //Debug.
    noTone(buzzerPin);//Debug. Turn off tone function.
    while(true); //Stop running. 
break;

case 3:
    Serial.print(F("ERROR[3]: Can not create file "));
    Serial.print(filename);
    Serial.println("."); //Error message.
    tone(buzzerPin, 494, 100);  //Debug.
    delay(200); //Debug.
    noTone(buzzerPin);//Debug. Turn off tone function.
    while(true); //Stop running.
break;


case 4:  
  Serial.print(F("ERROR[4]: Could not OPEN file "));
  Serial.print(filename);
  Serial.println(".");
  tone(buzzerPin, 494, 100);  //Debug.
  delay(200); //Debug.
  noTone(buzzerPin);//Debug. Turn off tone function.
  while(true); //Stop running.
break;


case 9:
    Serial.println(F("ERROR[9]: SD card initialization failed!"));
    Serial.println(F("Program execution is halted."));
    tone(buzzerPin, 494, 100);  //Debug.
    delay(200); //Debug.
    noTone(buzzerPin);//Debug. Turn off tone function.
    while(true); //Stop running.
break;


  } //End case statement.


}//End ErrorHandler.

