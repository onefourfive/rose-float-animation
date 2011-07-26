#include <SD.h>
#include <SPI.h>
#include <string.h>

File myPot; //File myFile;
int sensorPin = A0; //Pot Input
int sensorValue = 0;  // variable to store the value coming from the sensor
const int ssPin = 10;  //slave select pin
int ubyte;  // XX6-Bdata
int lbyte;  // 4-Bdata4bVS
byte VS=0;  //valve select
int WRITE = A1;  //enable for write
int READ = A2;  //enable for read

void setup()
{
  Serial.begin(9600); // bit rate
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(ssPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(WRITE, INPUT);
  pinMode(READ, INPUT);

  if (!SD.begin(ssPin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");   

  SPI.begin();
}

int sdWRITE(int VS)
{
  int i = 0;
  char asc[4];

  sensorValue = analogRead(sensorPin);
    ubyte=(sensorValue&0B1111110000)>>6;  //xx-6bitdata MSB
    lbyte=(((sensorValue&0B00001111)<<4)&0B11110000)|((VS)&0B00001111);  //4bitdataLSB-4bitVS
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myPot = SD.open("test.txt", FILE_WRITE); //myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myPot) 
  {
    //          Serial.print(sensorValue);
    //          Serial.println(" data");
    //          Serial.print(sensorValue,BIN);
    //          Serial.println(" data BIN");
    //          Serial.print(VS);
    //          Serial.println(" Valve #");
    //          Serial.print(VS,BIN);
    //          Serial.println(" Valve # Bin");

    for(i=3;i>=0;i--)
    {
      asc[i]=sensorValue%10+48;
      sensorValue=sensorValue/10;
      asc[7]=0;
      asc[6]=0;
      asc[5]=0;
      asc[4]=0;
    }
    Serial.print(asc); //print to screen
    Serial.println(VS);

    //          Serial.print(ubyte);
    //          Serial.println(" ubyte");
    //          Serial.print(ubyte, BIN);
    //          Serial.println(" ubyte BIN");
    //          Serial.print(lbyte);    
    //          Serial.println(" lbyte");
    //          Serial.print(lbyte,BIN);
    //          Serial.println(" lbyte BIN");

    myPot.print(asc); //write to sd card
    myPot.print(VS);
    //    Serial.println("Test file closed");
    delay(33);
    myPot.close();
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error WRITING test.txt");
  }
//    spiOUT(lbyte,ubyte);
}

void sdREAD()
{
  // re-open the file for reading:

  myPot = SD.open("test.txt");
  if (myPot)
  {
    int data = 0;
    int Valve = 0;
    int tdata[8] = {0};
    int temp = 0;

    //    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myPot.available())
    {
      int i = 0;
      //    int temp2=strlen(myPot.read());
      //      for(i=0;i<temp2;i++)
      for(i=0;i<6;i++)
      {
        //        temp = (myPot.read())-48;
        //        data = temp+data*10;
        tdata[i]=(myPot.read())-48;
        if(i==5)
        {
          data=tdata[0]*1000+tdata[1]*100+tdata[2]*10+tdata[3];  //*******************problem with leading zeros
          Valve=tdata[4]*10+tdata[5];
          ubyte=(data&0B1111110000)>>6;  //xx-6bitdata MSB
          lbyte=(((data&0B00001111)<<4)&0B11110000)|((Valve)&0B00001111);  //4bitdataLSB-4bitVS

          Serial.println(data);
          //          Serial.println(" data");
          //          Serial.print(data,BIN);
          //          Serial.println(" data BIN");
          Serial.println(Valve);
          //          Serial.println(" Valve #");
          //          Serial.print(Valve,BIN);
          //          Serial.println(" Valve # Bin");
          //          
          //          Serial.print(ubyte);
          //          Serial.println(" ubyte");
          //          Serial.print(ubyte, BIN);
          //          Serial.println(" ubyte BIN");
          //          Serial.print(lbyte);    
          //          Serial.println(" lbyte");
          //          Serial.print(lbyte,BIN);
          //          Serial.println(" lbyte BIN");
          spiOUT(lbyte,ubyte);
          delay(33);
        }
      }
    }
    // close the file:
    myPot.close();
  } 
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error READING test.txt");
  }
 
}

void loop()
{
  while(1)
  {
    VS = 15;
    //if (WRITE==1)
    sdWRITE(VS);  

    //  if (READ==1)
    //sdREAD();

  }
}

int spiOUT(byte lbyte, byte ubyte)
{
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(HIGH);
  //  SPI.setClockDivider(4);  //arduino clock 16MHz. clock divide results to 4MHz
  digitalWrite(ssPin,LOW);  //start spi
  SPI.transfer(ubyte);
  SPI.transfer(lbyte);
  digitalWrite(ssPin,HIGH);  //turn off spi
}




