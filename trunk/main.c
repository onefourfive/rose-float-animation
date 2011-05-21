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
	
	extern int desired_position, current_position;
	
	int PID_output;

	Initialize_ADC();

	configSPI();


	TRISAbits.RA0 = 1;
	TRISAbits.RA1 = 1;

	TRISAbits.RA7 = 0;  //slave select
    while(1){	


		desired_position = Get_ADC(1);  // Pin 3
				
		current_position = Get_ADC(0);  // Pin 2
		
		PID_output = PID();

		commandOut(PID_output,0);
	    
//sendtoDAC(0xB7FF,0);
  
/*
    PORTAbits.RA7 = 0;

		SSP1BUF = 0x1F;
		while(!SSP1STATbits.BF);
		SSP1BUF = 0x00;
		while(!SSP1STATbits.BF);

      PORTAbits.RA7 = 1;
*/		
		}
}
