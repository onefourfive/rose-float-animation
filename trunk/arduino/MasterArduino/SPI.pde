

void spi_init(){
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(HIGH);
	SPI.setClockDivider(4);
	SPI.begin();
}

void spiOUT(byte lbyte, byte ubyte){

  SPI.transfer(ubyte);
  SPI.transfer(lbyte);
  digitalWrite(SD_CS,HIGH);  //turn off spi
}
