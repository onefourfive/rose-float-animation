//Master Arduino

#include <SD.h>
#include <SPI.h>

//ADC Pins, for testing
#define sensor0 A0
#define sensor1 A1

//Digital Control Pins, for testing
#define readModeSel 10

int SD_CS = 10;
boolean readMode;			// !readMode = writeMode
int result = 0;
char index = 0;				// index eats arrays and loops for breakfast, everyday
char serialData[3];			// one 8 bit char for low byte, one for high byte
							// with char arrays, one more index is needed for null char

void setup()
{

  //what does what
  pinMode(SD_CS, OUTPUT);
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(readModeSel, INPUT);
  
  //Serial.magic();
  Serial.begin(9600); // bits/s
  Serial.print("Initializing SD card...");
  
  if(!SD.begin(SD_CS)){  	//SD.begin() returns false on failure
    Serial.println("Initialization Failed");
    return;
  }
  
}

void loop()
{
	readMode = digitalRead(readModeSel);
	if (readMode)
		readSD();
	else
		writeSD();
}

void writeSD(){

	File commandFile = SD.open("log.txt", FILE_WRITE);			//open log for writing

	if (Serial.available() > 0){
		index = 0;								//initialize index, get ready to do work
		
		while(Serial.available() != 0){			//while there's data to be had
			if(index < 3){						//save Serial bytes to char array serialData
				serialData[index++] = Serial.read();
			}
		}
			//what did we get?
			String outgoingData1 = String(serialData[0], BIN);	//print first byte in binary to string
			String outgoingData2 = String(serialData[1], BIN);	//then second byte
			totalData = outgoingData1 + outgoingData2;			//concatenate them for later use...
			
			Serial.print("I got " + totalData + ".");			//make it readable
	}
	
	
	
	if(commandFile){
		String cur_time = String(millis(), DEC);				//log time for giggles
		commandFile.print("at time " + cur_time + ":\t");		//
		commandFile.println(outgoingData1 + outgoingData2);		//remember things
		commandFile.close();									//close file (necessary?)
	}
  else
  {
    Serial.println("Couldn't access file");
  }
  
  delay(1000); 													//slow down
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
  digitalWrite(SD_CS,LOW);  //start spi
  SPI.transfer(ubyte);
  SPI.transfer(lbyte);
  digitalWrite(SD_CS,HIGH);  //turn off spi
}
