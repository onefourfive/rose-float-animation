#include <p18f46k22.h>
#include <stdio.h>
#include "rosefloat.h"



#pragma config 	FOSC = INTIO67		//internal oscillator
#pragma config	BOREN = OFF			//no brownout functions
#pragma config	WDTEN = OFF			//no watchdog timer
#pragma config PBADEN = ON			//on reset, PORTB = analog
#pragma config 	MCLRE = EXTMCLR		//MCLR pin works



int DAC_select(int);
int desired_position(int); 
int current_position(int);
int PID_output[2]={0,1};

void main ()
{	 
int i;

Initialize_ADC();
configSPI();


TRISAbits.RA0 = 1; //pin2=current possition 1 
TRISAbits.RA1 = 1; //pin3=desired possition 1

TRISAbits.RA3 = 1; //pin4=current possition 2
TRISAbits.RA4 = 1; //pin5=desired possition 2

TRISAbits.RA7 = 0;  //valve 1 enable Pin 13	
TRISAbits.RA6 = 0;  //valve 2 enable Pin 14

while(1)
	{
	for(i=1;i<3;i++)
		{
		PID_output[i] = PID(desired_position(i),current_position(i));
		commandOut(PID_output[i],DAC_select(i));
		}
	}
}



int desired_position(int a)
{
	int j;
	switch(a)
	{
	case 1: j=Get_ADC(1); break;
	case 2: j=Get_ADC(3); break;
//	case 3: j=Get_ADC(); break;
//	case 4: j=Get_ADC(); break;
//	case 5: j=Get_ADC(); break;
//	case 6: j=Get_ADC(); break;
//	case 7: j=Get_ADC(); break;
//	case 8: j=Get_ADC(); break;
//	case 9: j=Get_ADC(); break;
//	case 10: j=Get_ADC(); break;
//	case 11: j=Get_ADC(); break;
//	case 12: j=Get_ADC(); break;
//	case 13: j=Get_ADC(); break;
//	case 14: j=Get_ADC(); break;
//	case 15: j=Get_ADC(); break;
//	case 16: j=Get_ADC(); break;
	default: break;
	}	
	return j;
}




int current_position(int b)

{
	int k;
	switch(b)
	{
	case 1: k=Get_ADC(0); break;
	case 2: k=Get_ADC(2); break;
//	case 3: k=Get_ADC(); break;
//	case 4: k=Get_ADC(); break;
//	case 5: k=Get_ADC(); break;
//	case 6: k=Get_ADC(); break;
//	case 7: k=Get_ADC(); break;
//	case 8: k=Get_ADC(); break;
//	case 9: k=Get_ADC(); break;
//	case 10: k=Get_ADC(); break;
//	case 11: k=Get_ADC(); break;
//	case 12: k=Get_ADC(); break;
//	case 13: k=Get_ADC(); break;
//	case 14: k=Get_ADC(); break;
//	case 15: k=Get_ADC(); break;
//	case 16: k=Get_ADC(); break;	
	default: break;
	}	
	return k;
}




int DAC_select(int c)

{
	int l;
	switch(c)
	{
	case 1: l=0; break;
	case 2: l=1; break;
//	case 3: l=2; break;
//	case 4: l=3; break;
//	case 5: l=4; break;
//	case 6: l=5; break;
//	case 7: l=6; break;
//	case 8: l=7; break;
//	case 9: l=8; break;
//	case 10: l=9; break;
//	case 11: l=10; break;
//	case 12: l=11; break;
//	case 13: l=12; break;
//	case 14: l=13; break;
//	case 15: l=14; break;
//	case 16: l=15; break; 
	default: break;
	}	
	return l;
}

