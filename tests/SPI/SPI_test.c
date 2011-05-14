/*
PROJECT MAESTRO, ROSE FLOAT ANIM. COMPUTER
MAIN PROGRAM LOOP

BARE BONES, FOR TEST USE ONLY,
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


void main (){
	
	configSPI();
	
    while(1){	
	    
		
      
		commandOut(788,0);
		
		}
}	
