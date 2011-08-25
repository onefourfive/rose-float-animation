//Master Arduino

#include <SD.h>
#include <SPI.h>

//ADC Pins, for testing
#define sensor0 A0
#define sensor1 A1

//Digital Control Pins, for testing
#define readModeSel 10

//Pins reserved for SPI-- these are automatically set by SPI.begin()
/*
//MEGA
#define MISO 	50	
#define MOSI 	51
#define SCK	52
#define SS	53	//SS pin MUST be set as output
*/

//UNO
#define MISO 	12
#define MOSI 	11
#define SCK	13	//SS pin MUST be set as output
#define SD_CS	10


boolean readMode;			// !readMode = writeMode
int result = 0;
char index = 0;				// index eats arrays and loops for breakfast, everyday
char serialData[3];			// one 8 bit char for low byte, one for high byte
					// with char arrays, one more index is needed for null char

void setup(){

	//what does what
	pinMode(SD_CS, OUTPUT);
	pinMode(sensor0, INPUT);
	pinMode(sensor1, INPUT);
	pinMode(readModeSel, INPUT);
	
	//SPI.magic();
	spi_init();
	
	//Serial.magic();
	Serial.begin(9600);
	Serial.print("Initializing SD card...");
	
	if(!SD.begin(SD_CS)){  		//SD.begin() returns false on failure
		Serial.println("Initialization Failed");
		return;
	}
	else
		Serial.println("Initialization Success");
		

	//File commandFile = SD.open("log.txt", FILE_WRITE);	//open log for writing
  
}

void loop(){

	readMode = digitalRead(readModeSel);		//what to do
	
	if (readMode)
		readSD();
	else
		writeSD();
}

void writeSD(){
  
        String outgoingData1 = "";
        String outgoingData2 = "";
        String totalData = "";

	File commandFile = SD.open("log.txt", FILE_WRITE);	//opened in setup()

	if (Serial.available() > 0){
		index = 0;					//initialize index, get ready to do work
		
		while(Serial.available() != 0){			//while there's data to be had
			if(index < 3){				//save Serial bytes to char array serialData
				serialData[index++] = Serial.read();
			}
		}
			//what did we get?
			outgoingData1 = String(serialData[0], BIN);	//print first byte in binary to string
			outgoingData2 = String(serialData[1], BIN);	//then second byte
			totalData = outgoingData1 + outgoingData2;		//concatenate them for later use...
			
			Serial.print("I got " + totalData + ".");		//make it readable
	}
	
	
	if(commandFile){
		String cur_time = String(millis(), DEC);			//log time for giggles
		commandFile.print("at time " + cur_time + ":\t");		//
		commandFile.println(totalData);		//remember things
		//commandFile.close();						//close file (necessary?)
	}
	else{
		Serial.println("Couldn't access file");
	}
  
  delay(1000); 		//slow down
  
}

void readSD(){
	File commandFile = SD.open("log.txt");	//opened in setup()
	Serial.println("Read Mode");
  
	if (commandFile){
		Serial.println("Reading Command File");
		int flag = 0;
		while(commandFile.available()){
			int temp = commandFile.read();
			// Serial.println(temp-48);
			if(temp == 10 || temp == 13){
				if ( flag != 1){
					Serial.println(result);
					flag = 1;
					result = 0;
					delay(500);
				}
	      
			}
			else if( temp == 32){
				if ( flag != 1){
					String temp1 = String(result) + ' ';
					Serial.print(temp1);
					flag = 1;
					result = 0;
				}
			}
			else{
				flag = 0;
				result = (result*10)+(temp-48);
			}
		}
	}
	else{
		Serial.println("Read failed.");
		//return;
	}
}


