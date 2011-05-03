/*
A		D		C	

S	T	U	F	F
======================

Derek 04/13/11
Sean 4/23/11
Derek 05/03/11 - Put the ADC functions here.
*/
void Initialize_ADC(void); 
int Get_ADC(int);
int PID(void);


int Kp,Ki,Kd,new_error,old_error,P_err,I_err,D_err;


#include <p18f46k22.h>

int PID( void )
{
	int dp = 0;
	int cp = 0;

	old_error = new_error;
	new_error = dp-cp;
	P_err = new_error;
	I_err += old_error;
	D_err = new_error - old_error;
	
	return (1*P_err + 1*I_err + 1*D_err); //Gains set to 1 for testing

}
int Get_ADC( int i )
{
	//ADCON0 = 0x03; //Selects ADChannel "i" and turns on ADC
	ADCON0 = (i << 4) + 3;
	ADCON0bits.GO = 1; // Start conversion
	while(ADCON0bits.DONE==1); // Wait for completion
	ADCON0bits.ADON = 0;  // Disable A/D converter
	return (((int)ADRESH << 8) + ADRESL); //ReadADC(); // Read result

}

void Initialize_ADC(void)	// configure A/D convertor
{
	ADCON0=0x03; 
    ADCON1=0x1D; 
    ADCON2=0x29; 
}