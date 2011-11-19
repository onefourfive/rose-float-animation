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


unsigned int V1[50];
unsigned int V2[50];

void setup(void);
void loop(void);
void timer(void);
void reset_timer(void);
void sine_wave(void);
void record(void);
void playback(void);
int read_EEPROM(unsigned int);
void write_EEPROM(unsigned int, unsigned int);


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
	CCP3CON = 0x00;

   /*PORT DIRECTIONS FOR LEARNING & PLAY BACK*/
	TRISAbits.RA5 = 1;	//Learn/Record
	TRISEbits.RE0 = 1;	//Play back
	TRISEbits.RE1 = 1;	//
	TRISEbits.RE2 = 1;	//
	
}

void loop(){

int i;

	while(1)
	{

	if(PORTEbits.RE0==1){
			playback();
		}
	else if(PORTAbits.RA5==1){
			record();
	//		commandOut(0,0);
	//		for(i=5000;i>0;i--)
	//		{}
	//		playback();
	//		sine_wave();
		}
//		if(PORTEbits.RE0==1){
//			record();
//		}
		else
		{
		//	commandOut(getCurrentPosition(2)-getCurrentPosition(1),0);
		//	commandOut(getCurrentPosition(2),0);
			commandOut(-1023,0);
		}
	}
}	

void timer(){

	T0CON = 0b10010111;
}

void reset_timer(){

	TMR0L = 0b11010111;
	T0CON = 0b10010111;
}

void sine_wave(){

int i=0,d=0,k=2000,count=0;

while(count<2500)
{
	if(d==0)
			i++;
			else
			i--;
			commandOut(i-getCurrentPosition(0),0);
			count++;
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
}

void record(){
int k=0,i;
		for(k=0;k<50;k++)
{
		V1[k]=getCurrentPosition(2);
		commandOut(PID(V1[k],getCurrentPosition(0)),0);
		for(i=12000;i>0;i--)
		{}

write_EEPROM(getCurrentPosition(2),k*2);

}
}


void playback(){
int k=0,i;
		for(k=0;k<50;k++)
{
		commandOut(V1[k]-getCurrentPosition(0),0);
		for(i=12000;i>0;i--)
		{}
}
}


int read_EEPROM(unsigned int reg){


EEADR = reg & 0x00FF;
EEADRH = (reg>>8) & 0x03; 

EECON1bits.CFGS = 0;
EECON1bits.EEPGD = 0;
EECON1bits.RD = 1;

return EEDATA;
}



void write_EEPROM(unsigned int data, unsigned int reg){

EEDATA = data & 0x00FF;
EEADR = reg & 0x00FF;
EEADRH = (reg>>8) & 0x03; 

EECON1 = 0b00001100;
INTCONbits.GIE = 0;
EECON2 = 0x55;
EECON2 = 0x0A;
EECON1 = 0b00001110;
INTCONbits.GIE = 1;
EECON1bits.WREN = 0;

//writing to next byte

reg++;

EEDATA = (data>>8) & 0x03;
EEADR = reg & 0x00FF;
EEADRH = (reg>>8) & 0x03; 

EECON1 = 0b00001100;
INTCONbits.GIE = 0;
EECON2 = 0x55;
EECON2 = 0x0A;
EECON1 = 0b00001110;
INTCONbits.GIE = 1;
EECON1bits.WREN = 0;

}