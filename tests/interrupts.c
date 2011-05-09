/*
	Interrupt Configuration
	---------------------------
	
	We would like to generate an interrupt every
	1/30th of a second to provide the 30Hz framerate
	we're aiming for.
	
	configInterrupts()	::	Set the right bits to enable the
							SPI2 interrupt for incoming data +
							TMR0 interrupt for syncing outputs.
	
	Interrupts used:
	
	INTCONbits.TMR0IF = TMR0 flag
	INTCONbits.INT0IF = External Interrupt 0 Flag
	PIR3bits.SSP2IF = Transfer complete on SSP2
	
	Derek Sorenson, 5/2/11
	
	Changelog
	v0.1	Included both external INT0 interrupt to come from
			arduino and TMR0 interrupt to be used internally. We
			can choose which to use later
	
*/

/*	
	This pragma instruction tells the compiler to put this code at 0008h,
	where all interrupts (while priority interrupts are disabled)
	will go.
*/

#include <p18f4550.h>
#include "rosefloat.h"

void chk_isr(void);

#pragma code high_vector=0x08
void isr(void){
	_asm
		GOTO chk_isr
	_endasm
}

#pragma interrupt chk_isr
void chk_isr(){

	//if(PIR3bits.SSP2IF)
		//getRefdata();
//	if(INTCONbits.TMR0IF)
//		sendtoDAC(0,0);
//	if(INTCONbits.INT0IF)
//		sendtoDAC(0,0);

}
#pragma code
void configInterrupts(void){
	
	RCONbits.IPEN = 0;		//disable priority levels
	INTCONbits.GIE = 1;		//enable global interrupts
	INTCONbits.PEIE = 1;	//enable peripheral interrupts
	INTCON2bits.INTEDG0 = 1;	//INT0 Interrupt on rising edge

	
	/*Peripheral Interrupt Enable Bits*/
	//PIE3bits.SSP2IE = 1;	//enable SPI ch 2 (slave)
	INTCONbits.TMR0IE = 1;	//enable TMR0 interrupt
	INTCONbits.INT0IE = 1;	//enable ext. interrupt 0 (pin 33)
	
}

void configTimer0(void)	{

	/*Config TMR0CON*/
	
	T0CONbits.TMR0ON = 0;		//according to docs, TMR0 defaults on
	T0CONbits.PSA = 1;		//enable prescale
	//T0CONbits.T0PS = 0b001;	//prescale of 4
	T0CONbits.T0CS = 0;		//rely on instruction cycle clock
	T0CONbits.T08BIT = 0;		//16-bit mode
	
	T0CONbits.TMR0ON = 1;		//let's go
	
}

void resetTimer0(void){
	
	/*
	With 16MHz clock, Tcyc = 4MHz. 1/(4MHz) = .25 us per tick
	There are 133,333.33 of these ticks in 1/30 of a second
	Therefore, with prescale value 1:4, it will take
	33,333.33 (~33,333) ticks to count up to 1/30 of a second.
	
	Thus we load TMR0 with 2^16 - 33,333 = 32,202 = 0x7DCA.
	*/
	
	T0CONbits.TMR0ON = 0;		//stop
	INTCONbits.TMR0IF = 0;		//clear interrupt flag
	TMR0H = 0x7D;				//0x7DCA
	TMR0L = 0xCA;
	T0CONbits.TMR0ON = 1;		//go

}
	
	