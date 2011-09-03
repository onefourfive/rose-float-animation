/*
A		D		C	

S	T	U	F	F
======================

Derek 04/13/11
Sean 4/23/11
Derek 05/03/11 - Put the ADC functions here.
*/



int Kp,Ki,Kd,new_error,old_error,P_err,I_err,D_err;

int desired_position, current_position;


#include <p18f46k22.h>
#include "rosefloat.h"




int PID(int old_error, int new_error)
{

	old_error = new_error;
	new_error = desired_position-current_position;
	P_err = new_error;
	I_err += old_error;
	D_err = new_error - old_error;
	
	return (1*P_err + 0*I_err + 0*D_err); //Gains set to 1 for testing

}
int Current_Position( int i )
{
  int k;
	switch(i)
	{
	case 0: k=0; break;
	case 1: k=1; break;
	case 2: k=2; break;
//	case 3: k=3; break;
//	case 4: k=4; break;
//	case 5: k=5; break;
//	case 6: k=6; break;
//	case 7: k=7; break;
//	case 8: k=8; break;
//	case 9: k=9; break;
//	case 10: k=10; break;
//	case 11: k=11; break;
//	case 12: k=13; break;
//	case 13: k=14; break;
//	case 14: k=16; break;
//	case 15: k=25; break;	
	default: break;
	}	
	//Selects ADChannel "k" and turns on ADC
	ADCON0 = (k << 2) + 3;
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
	ADCON2bits.ACQT = 0b011;	//16Tad  NEEDS More time to acq
 	ADCON2bits.ADCS = 0b101;	//Fosc/16
	
	/*Configure ANSEL pins*/
	ANSELA = 0b00101111;
	ANSELB = 0b00111110;
	ANSELC = 0b11011100;
	ANSELD = 0b00000010;
	ANSELE = 0b00000111;
}