/*
	Interrupt Configuration
	---------------------------
	
	We would like to generate an interrupt every
	1/30th of a second to provide the 30Hz framerate
	we're aiming for.
	
	Derek Sorenson, 5/2/11
*/

void configInterrupts(void){
	
	RCONbits.IPEN = 0;		//disable priority levels
	INTCONbits.GIE = 1;		//enable global interrupts
	INTCONbits.PEIE = 1;	//enable peripheral interrupts
	
	/*Peripheral Interrupt Enable Bits*/
	PIE2bits.SSP2IE = 1;	//enable SPI ch 2 (slave)
	INTCONbits.TMR0IE = 1;	//enable TMR0 interrupt

}

void configTimer0(void)	{
	//timer stuff here
}
	
	