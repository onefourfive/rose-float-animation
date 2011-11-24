/*Do not modify!


EEPROM CODE



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
#pragma config 	WRTD 	= OFF			//Data EEPROM not write-protected 
#pragma config 	CPD 	= OFF			//Data EE Read Protect Disabled

unsigned int V1[50];
unsigned int V2[50];

void setup(void);
void loop(void);
void timer(void);
void reset_timer(void);
void sine_wave(void);
void record(void);
void record_eep(void);
void playback_eep(void);

//EEPROM functions
void Write_eep( unsigned int,unsigned char);
unsigned char Read_b_eep( unsigned int);

void main (){
int k,i;
	//need a way to start control loop with refData,
	//but keep running until interrupt calls refData again
	setup();

//commandOut(512,0);

//	commandOut(getCurrentPosition(0),0); 
//	commandOut(512,0);

	loop();
//write_EEPROM(256,30);
//write_EEPROM(512,40);


// write_char_EEPROM(234,1);
// k=(int)read_EEPROM(0);
// WriteEEPROM(0,234);
//k = ReadEEPROM(0);
// EECON1=0;
//Write_eep(1,255);

//Write_eep(0,128);
//Write_eep(2,64);

//k=Read_b_eep(1);
//commandOut(((int)k & 0x00FF),0);
//commandOut(((int)Read_b_eep(0)),0);
/*
while(1){
		for(k=0;k<4;k++)
{
		commandOut(((int)Read_b_eep(k)),0);
		for(i=20000;i>0;i--)
		{}
}
}
*/
////
}	

void setup(){
	
	configSPI();
	Initialize_ADC();
//	timer();
//	reset_timer();
	CCP3CON = 0x00;
	INTCON = 0x00;

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
			playback_eep();
		}
	else if(PORTAbits.RA5==1){
			record_eep();
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
		//	commandOut(getCurrentPosition(0),0);
			commandOut(512,0);
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

//write_EEPROM(getCurrentPosition(2),k*2);

}
}

void record_eep(){
int k=0,i,l;
		for(k=0;k<50;k++)
{
		l=getCurrentPosition(0);
		Write_eep(k*2,ADRESL);
		Write_eep(k*2+1,ADRESH);
		commandOut(l,0);
		for(i=5000;i>0;i--)
		{}


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

void playback_eep(){
int k=0,i,b;
char l,h;
		for(k=0;k<50;k++)
{
	//	l=Read_b_eep(k*2);
	//	h=Read_b_eep(k*2+1);
	//	b=(((int)h << 8) + l);
		commandOut((((int)Read_b_eep(k*2+1) << 8) + Read_b_eep(k*2))-getCurrentPosition(0),0);
		for(i=5000;i>0;i--)
		{}
}
}



void Write_eep( unsigned int badd,unsigned char bdata )
{
/*
#asm
	BCF EECON1, EEPGD// ; Point to DATA memory
	BCF EECON1, CFGS ; Access EEPROM
	BSF EECON1, WREN ; Enable writes
	BCF INTCON, GIE ; Disable Interrupts
	MOVLW 55h ;
	Required MOVWF EECON2 ; Write 55h
	Sequence MOVLW 0AAh ;
	MOVWF EECON2 ; Write 0AAh
	BSF EECON1, WR ; Set WR bit to begin write
	BSF INTCON, GIE ; Enable Interrupts
	; User code execution
	BCF EECON1, WREN ;
#endasm
*/
//	EECON1 = 0;
	EEADRH = (unsigned char)(badd >> 8);
	EEADR = (unsigned char)(badd & 0x0ff);
	EEDATA = bdata;



  	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;
	INTCONbits.GIE = 0;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	while(EECON1bits.WR);				//Wait till the write completion
//	while(!PIR2bits.EEIF);                // Wait for write to complete
//    PIR2bits.EEIF=0;	
	EECON1bits.WREN = 0;
	INTCONbits.GIE = 1;
///////////////


}

unsigned char Read_b_eep( unsigned int badd )
{
	EEADRH = (badd >> 8) & 0x03;
	EEADR = (badd & 0x0ff);
  	EECON1bits.CFGS = 0;
	EECON1bits.EEPGD = 0;
	EECON1bits.RD = 1;
	Nop();							//Nop may be required for latency at high frequencies
	Nop();							//Nop may be required for latency at high frequencies
	return ( EEDATA );              // return with read byte 
}