//Master Arduino

#include <SD.h>
#include <SPI.h>

int CS_pin = 10;
int sensorPin = 0; //Pot Input valve 0
int sensorPin1 = 1; //Pot input valve 1
int WriteMode = 2;
int ReadMode = 3;
int result = 0;
byte ubyte = 0;
byte lbyte = 0;
String incomingDATA = "";	// for incoming serial data

void setup()
{
  
  Serial.begin(9600); // bits/s
  //Serial.print("Initializing SD card...");
  
  pinMode(CS_pin, OUTPUT);
  pinMode(sensorPin, INPUT);
  
  if(!SD.begin(CS_pin))
  {
    Serial.println("Card Failed");
    return;
  }
  
}

void loop()
{
  int myread = analogRead(ReadMode);
  int mywrite = analogRead(WriteMode);
 // if ( myread == 1 && mywrite == 0)
  //{
   // readSD();
  //}
  //else if ( myread == 0 && mywrite == 1)
  //{
    writeSD();
  //}
  //else
  //Serial.println("WTF");
}

int writeSD()
{
if (Serial.available() > 0) 
{
		// read the incoming byte:
		String incomingByte = Serial.read();

		// say what you got:
		Serial.println(incomingDATA);
}
  File commandFile = SD.open("log.txt", FILE_WRITE);
  if(commandFile)
  {
    commandFile.println(incomingDATA);
    commandFile.close();
    Serial.println(incomingDATA);
  }
  else
  {
    Serial.println("Couldn't access file");
  }
  
  delay(1000); 
}

int readSD()
{
  File commandFile = SD.open("log.txt");
  if (commandFile)
  {
    //Serial.println("Reading Command File");
    int flag = 0;
   while(commandFile.available())
  {
    int temp = commandFile.read();
   // Serial.println(temp-48);
    if(temp == 10 || temp == 13)
    {
      if ( flag != 1)
      {

        Serial.println(result);
        flag = 1;
        result = 0;
        delay(500);
      }
      
    }
    else if( temp == 32)
    {
      if ( flag != 1)
      {
        
        String temp1 = String(result) + ' ';
        Serial.print(temp1);
        flag = 1;
        result = 0;
      }
    }
    else
      {
        flag = 0;
        result = (result*10)+(temp-48);
      }
      
    }
   
  }
  else 
   {
     Serial.println("Could not read Command File.");
     //return;
   } 
}
int spiOUT(byte lbyte, byte ubyte)
{
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(HIGH);
  //  SPI.setClockDivider(4);  //arduino clock 16MHz. clock divide results to 4MHz
  digitalWrite(CS_pin,LOW);  //start spi
  SPI.transfer(ubyte);
  SPI.transfer(lbyte);
  digitalWrite(CS_pin,HIGH);  //turn off spi
}
