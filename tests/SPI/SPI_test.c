/*
PROJECT MAESTRO, ROSE FLOAT ANIM. COMPUTER
MAIN PROGRAM LOOP

BARE BONES, FOR TEST USE ONLY,
Reads 1 ADC.
Output SPI to 1 DAC.
Repeats continuously. No interrupts used.
*/

#include <p18f46k22.h>
#include "C:\Documents and Settings\Electronics\Desktop\rose-float-animation\tests\rosefloat.h"

#pragma config 	FOSC = INTIO67		//internal oscillator
#pragma config	BOREN = OFF			//no brownout functions
#pragma config	WDTEN = OFF			//no watchdog timer
#pragma config	PBADEN = ON			//on reset, PORTB = analog
#pragma config 	MCLRE = EXTMCLR		//MCLR pin works



void main (){
	
//	configSPI();

	int PW = 512;	
	char buf1,buf2;
	//SSP1 : SPI Master Mode for DACs
	SSP1STATbits.SMP = 1;		//Input data sampled at end of data output time
	SSP1STATbits.CKE = 0;		//Transmit occurs on idle->active transition
	
	SSP1CON1bits.SSPEN = 1;	//Serial port enable
	SSP1CON1bits.CKP = 0;
	SSP1CON1bits.SSPM = 0b0001;  //SPI frequency = Fosc / 16
	
	
	/*PORT DIRECTIONS FOR SPI TO WORK*/
	TRISCbits.RC3 = 0;	//SCK1
	TRISCbits.RC4 = 1;	//SDI1
	TRISCbits.RC5 = 0;	//SDO1
	TRISEbits.RE0 = 0;  //slave select
	

    while(1){	
	    
		buf1=0x30; //configBits
		buf1= buf1|(PW>>6);
		buf2=0x00;
		buf2=buf2|(PW&0x003F);

      PORTEbits.RE0 = 0;
		//CHN A or B , 
	SSP1BUF = buf1;
	while(!SSP1STATbits.BF);
	SSP1BUF = buf2;
	while(!SSP1STATbits.BF);

      PORTEbits.RE0 = 1;
		
		}
}
