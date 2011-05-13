/*
PROJECT MAESTRO, ROSE FLOAT ANIM. COMPUTER
MAIN PROGRAM LOOP

BARE BONES, FOR TEST USE ONLY, SO FAR
No SPI to Big Chip.
Reads 1 ADC.
Output SPI to 1 DAC.
Repeats continuously. No interrupts used.
*/

#include <p18f46k22.h>
#include "rosefloat.h"

#pragma config 	FOSC = INTIO67		//internal oscillator
#pragma config	BOREN = OFF			//no brownout functions
#pragma config	WDTEN = OFF			//no watchdog timer
#pragma config	PBADEN = ON			//on reset, PORTB = analog
#pragma config 	MCLRE = EXTMCLR		//MCLR pin works


//unsigned char i=0;
int PID_output;

#pragma code high_vector=0x08

void isr(void){
	_asm
		GOTO chk_isr
	_endasm
}

#pragma interrupt chk_isr

void chk_isr(void){

	if(PIR3bits.SSP2IF)
		getRefdata();
	if(INTCONbits.TMR0IF)
		sendtoDAC(0, 0);	//these two
	if(INTCONbits.INT0IF)
		sendtoDAC(0, 0);	//sendtoDAC calls
		
	//don't do anything.. on these interrupts, we'd like to start
	//setting ALL outputs......

}

#pragma code

void main (){
	
	Initialize_ADC();
	configSPI();
	configInterrupts();
	configTimer0();
	

	//************PWM******************
	// One while(1) loop is 1 Frame
    while(1){	
	    
		/*
		SPI modules to get Desired Position from Big Chip
		getRefdata();
		*/		
		desired_position = Get_ADC(1);
				
		current_position = Get_ADC(0);
		
		PID_output = PID();
		
		commandOut(PID_output,0);
		
		}
		//while(new_frame==0);  //30Hz external clock
}	
