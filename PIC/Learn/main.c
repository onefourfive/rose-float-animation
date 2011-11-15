/*Do not modify!






This code belongs to Sean Ryan Conant.



Two valves with record ONLY

No SPI from arduino

SPI to DACs




*/


/*
PROJECT MAESTRO, ROSE FLOAT ANIM. COMPUTER
MAIN PROGRAM LOOP

Sean Conant 3 September 2011

Fix ADC ACQ and preparing for multiples valves

Repeats continuously.
*/

#include <p18f46k22.h>
#include "rosefloat.h"

/*
to view all pragma configs, 
go into command prompt and type
mcc18 -p18f46k22 --help-config
*/

#pragma config 	FOSC 	= INTIO67		//internal oscillator
#pragma config	BOREN 	= OFF			//no brownout functions
#pragma config	WDTEN 	= OFF			//no watchdog timer
#pragma config	PBADEN 	= ON			//on reset, PORTB pins in ANSELB are analog
#pragma config 	MCLRE 	= EXTMCLR		//MCLR pin works

#define rec PORTAbits.RA5
#define playback PORTEbits.RE0


unsigned int V1[50];
unsigned int V2[50];

void setup(void);
void loop(void);
void timer(void);
void reset_timer(void);





void main (){

	//need a way to start control loop with refData,
	//but keep running until interrupt calls refData again
	setup();
	loop();
}	

void setup(){
	
	configSPI();
	Initialize_ADC();
	timer();
	reset_timer();

   /*PORT DIRECTIONS FOR LEARNING & PLAY BACK*/
	TRISAbits.RA5 = 1;	//Learn/Record
	TRISEbits.RE0 = 1;	//Play back
	TRISEbits.RE1 = 1;	//
	TRISEbits.RE2 = 1;	//
	
}

void loop(){

int i=0,d=0,k=2000;


while(1)
{
if(d==0)
i++;
else
i--;
commandOut(i-getCurrentPosition(0),0);
if(i>1000)
d=1;
if(i<0)
d=0;
while(1)
{
for(k=100;k>1;k--)
{}
break;
}
}
/* 888888888888888888
	while(1){
		i=0;
		while(rec)
		{
		V1[i]=Current_Position(2);
		V2[i]=Current_Position(3);
		commandOut(PID(Current_Position(0),V1[i]),0);
		commandOut(PID(Current_Position(1),V2[i]),1);
		i++;
		if(i==50)
		break;
		while(!TMR0IF)
		{}
		reset_timer();
		}
		
		while(playback)
		{		
		commandOut(PID(Current_Position(0),V1[i]),0);
		commandOut(PID(Current_Position(1),V2[i]),1);
		i++;
		if(i==50)
		break;
		while(!TMR0IF)
		{}
		reset_timer();
		}
//		Current_Position(0);

	//	commandOut(522,0);
	
	//	commandOut(PID(Current_Position(0),Current_Position(2)),0);

	//	commandOut(PID(Current_Position(1),Current_Position(3)),1);
	}	
888888888888
*/
}	

void timer(){

	T0CON = 0b10010111;
}

void reset_timer(){

	TMR0L = 0b11010111;
	T0CON = 0b10010111;
}