/*
PROJECT MAESTRO, ROSE FLOAT ANIM. COMPUTER
MAIN PROGRAM LOOP

Sean Conant 3 September 2011

Fix ADC ACQ and preparing for multiples valves

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
	
   //Define ADC I/O
   
   int i=0;
   
	extern int desired_position;

	Initialize_ADC();

  //Define SPI I/O

	configSPI();

 //************* May need to initialize ADC ports....
	//TRISAbits.RA0 = 1;
  // TRISAbits.RA1 = 1;

   
   
    while(1){	

      //SPI in
   
      for(i=0;i<16;i++)
       { 
         commandOut(PID(desired_position(i),Current_Position(i),i);
       }
  
		}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//This will need to be done by SPI in
///////////////////////

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


