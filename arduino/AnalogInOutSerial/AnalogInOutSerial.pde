/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 Modified 4 Sep 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 */
 
 #include <SPI.h>

// These constants won't change.  They're used to give names
// to the pins used:
const int slaveSelectPin = 10;
const int analogInPin1 = 0;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = 1;
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensor1Value = 0;        // value read from the pot
int sensor2Value = 0;
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  pinMode(slaveSelectPin, OUTPUT);
  Serial.begin(9600);
  SPI.setBitOrder(LSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();
  
}

void loop() {
  // read the analog in value:
  sensor1Value = analogRead(analogInPin1); 
  sensor2Value = analogRead(analogInPin2);  
  // map it to the range of the analog out:
  outputValue = map(sensor1Value, 0, 1023, 0, 255);  
  // change the analog out value:
  Serial.println();     
     outToDAC();

  // print the results to the serial monitor:

  
  //Serial.print("\t output = ");      
  //Serial.println(outputValue);   

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10);                     
}

void outToDAC(byte high, byte low){
    byte high, low;
    
    for( byte low = 0xFF; low<=0xFF; low+4){
    digitalWrite(slaveSelectPin,LOW);
    Serial.print("sending byte high = ");
    Serial.print(high, HEX);
    SPI.transfer( high);
    Serial.print(" sending byte low = ");
    Serial.print(low, HEX);
    Serial.print(" ");
    SPI.transfer( low);
    digitalWrite(slaveSelectPin,HIGH);
      Serial.print("sensor1 = " );                       
    Serial.print(sensor1Value);
    Serial.print(" sensor2 = ");
    Serial.println(sensor2Value);
    }
}
