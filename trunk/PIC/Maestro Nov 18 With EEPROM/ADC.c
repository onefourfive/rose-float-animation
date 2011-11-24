/*
A		D		C	

S	T	U	F	F
======================

Derek 04/13/11
Sean 4/23/11
Derek 05/03/11 - Put the ADC functions here.
*/


int desired_position, current_position, P;


#include <p18f46k22.h>
#include "rosefloat.h"




int PID(unsigned int new_error, unsigned int old_error)
{	
	P=new_error-old_error;

	return (P); //Gains set to 1 for testing

}
int getCurrentPosition( unsigned char i )
{
	//Selects ADChannel "k" and turns on ADC
	ADCON0 = (i << 2) + 3;
//	ADCON0bits.GO = 1; // Start conversion
	while(ADCON0bits.GO==1); // Wait for completion
	ADCON0bits.ADON = 0;  // Disable A/D converter
	return (((int)ADRESH << 8) + ADRESL); //ReadADC(); // Read result

}

void Initialize_ADC(void)	// configure A/D convertor
{
	ADCON0bits.CHS = 0b0000;
	ADCON0bits.ADON = 0b0;

    ADCON1bits.PVCFG = 0b00;
	ADCON1bits.NVCFG = 0b00;

	ADCON2bits.ADFM = 1;	//RIGHT JUSTIFY
	ADCON2bits.ACQT = 0b111;	//16Tad  NEEDS More time to acq
 	ADCON2bits.ADCS = 0b101;	//Fosc/16

	
	/*Configure ANSEL pins*/ 
//for 16 ADCs
/*	ANSELA = 0b00101111;
	ANSELB = 0b00111110;
	ANSELC = 0b11011100;
	ANSELD = 0b00000010;
	ANSELE = 0b00000111;
*/

//for 4 ADCs, learning
	ANSELA = 0b00001111;
	ANSELE = 0b00000000;

}