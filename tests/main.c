/*
PROJECT MAESTRO, ROSE FLOAT ANIM. COMPUTER
MAIN PROGRAM LOOP

This file is intended to control 1 valve.  
No SPI to Big Chip.
Reads 1 ADC.
Output SPI to 1 DAC.
Repeats continuously.

No libraries are used because the intended p18F46k22 does not support them.
*/

#include <p18f4550.h>
#include "rosefloat.h"
#include <pwm.h>
/*configs for p18f46k22. ignore

#pragma config 	FOSC = INTIO67		//internal oscillator
#pragma config	BOREN = OFF			//no brownout functions
#pragma config	WDTEN = OFF			//no watchdog timer
#pragma config	PBADEN = ON			//on reset, PORTB = analog
#pragma config 	MCLRE = EXTMCLR		//MCLR pin works
*/

#pragma config FOSC = INTOSCIO_EC

#define DAC_B		0X8000	//bit masks for setting config bits
#define	DAC_A		0X0000	//in 16-bit DAC string.
#define DAC_B_OFF	0X9000
#define DAC_A_OFF	0X1000

int Kp,Ki,Kd,new_error=0,old_error=0,P_err,I_err,D_err;
int current_position,desired_position;
int PID_output;

//unsigned char i=0;

void Initialize_ADC(void); //Set up ADC registers: ADCON0, ADCON1, ADCON2

int Get_ADC(int);//Starts, finishes, and returns ADC

int PID(void);//

void configSPI(void);				//set up SPI registers
void sendtoDAC(unsigned int, unsigned char);
void commandOut(int, unsigned char);//output command signal
void getRefdata(void);				//read in 10-bit position data, gains



void main (){

	int i;
	int direction;
	TRISD = 0;
	configTimer0();
	Initialize_ADC();
//	configSPI();


	//************PWM******************
	// One while(1) loop is 1 Frame
    while(1){	
	    
		/*
		SPI modules to get Desired Position from Big Chip
		getRefdata();
		*/		
		desired_position = Get_ADC(1);
				
		current_position = Get_ADC(0);
		
		PID_output = PID();
		
		if(PID_output >= 0){					
			direction=0;		
		}
		else{
			direction=1;		
			PID_output *= -1;
		}

	if(PID_output > 1023){
	PID_output=1023;
	}

	if (direction==0)
		{PORTDbits.RD0=0;
		PORTDbits.RD1=1;
		}
	else
		{PORTDbits.RD0=1;
		PORTDbits.RD1=0;
		}

	OpenPWM1(0xA0);
	SetDCPWM1(PID_output);
	
	
		//while(INTCONbits.TMR0IF){}

		resetTimer0();
	//	commandOut(PID_output,0);
		//*if(;!=;)replace with -> ;;;;;;;;;;;;;; for joel

		}
		//while(new_frame==0);  //30Hz external clock
}


//************************************************************************************
//END MAIN
//************************************************************************************

int PID( void )
{
	old_error = new_error;
	new_error = desired_position-current_position;
	P_err = new_error;
	I_err  = 0;
	D_err = new_error - old_error;
	
	return (1*P_err + 1*I_err + 1*D_err); //Gains set to 1 for testing

}
int Get_ADC( int i )
{
	//ADCON0 = 0x03; //Selects ADChannel "i" and turns on ADC
	ADCON0 = (i << 2) + 3;
	ADCON0bits.GO = 1; // Start conversion
	while(ADCON0bits.DONE==1); // Wait for completion
	ADCON0bits.ADON = 0;  // Disable A/D converter
	return (((int)ADRESH << 8) | ADRESL); //ReadADC(); // Read result

}

void Initialize_ADC(void)	// configure A/D convertor
{
	ADCON0=0x03; 
    ADCON1=0x0D; 
    ADCON2=0xA9;
}

void commandOut(int voltage, unsigned char DACn){

	int dacdata = 0x0000;				//16 bit output string
	unsigned int DAC_on, DAC_off;		//config bit mask for DAC

	//What direction are we going in?
	if(voltage >= 0){					
		DAC_on = DAC_A;		//going up, use DAC_A
	}
	else if(voltage < 0){
		DAC_on = DAC_B;		//going down, use DAC_B
		voltage *= -1;
	}

	if(voltage > 1023){
	voltage=1023;
	}
	
	voltage = voltage & 0x03FF;	//ensure only 10 bit output

	dacdata = DAC_on;		//set config bits
	voltage = voltage << 2;			
	dacdata = dacdata | voltage;	//or in voltage data
	DAC_off = DAC_on ^ 0x9000;		//XOR turns off opposite channel
	
	sendtoDAC(dacdata, DACn);
	
	
}

void sendtoDAC(unsigned int data, unsigned char DACn){
	
	unsigned char high, low;
	high = (data >> 8);
	low = (data << 8);
	
//	PORTA = DACn;	//select w/ mux
	SSPBUF = high;
	while(!SSPSTATbits.BF);
	SSPBUF = low;
	while(!SSPSTATbits.BF);
//	PORTA = 0xF;
	
}

void configSPI(){
	
	/*
	Setting config registers SSPx for SPI. 
	Call this function from the	setup() function
	*/
	
	//SSP1 : SPI Master Mode for DACs
	SSPSTATbits.SMP = 0;		//Input data sampled at end of data output time
	SSPSTATbits.CKE = 0;		//Transmit occurs on idle->active transition
	
	SSPCON1bits.SSPEN = 1;	//Serial port enable
	//SSPCON1bits.SSPM = 0b0001;  //SPI frequency = Fosc / 16

	SSPCON1bits.SSPM0	=	1;
	SSPCON1bits.SSPM1	=	0;
	SSPCON1bits.SSPM2	=	0;
	SSPCON1bits.SSPM3	=	0;

//	//SSP2 : SPI Slave Mode (~SS Enabled) for receiving position data
//	SSP2STATbits.SMP = 0b0;
//	SSP2STATbits.CKE = 0b0;
//	SSP2CON1bits.SSPEN = 1;
//	SSP2CON1bits.SSPM = 0b0100;

	
	/*PORT DIRECTIONS FOR SPI TO WORK*/
	TRISCbits.TRISC7 = 0;	//SCK1
	TRISBbits.TRISB0 = 1;	//SDI1
	TRISBbits.TRISB1 = 0;	//SDO1
	
//	TRISDbits.RD0 = 1;	//SCK2
//	TRISDbits.RD1 = 1;	//SDI2
//	TRISDbits.RD3 = 1;	//SS 2
//	TRISDbits.RD4 = 0;	//SD02

}

















////****************************


/*
//Microchip Libraries
#include <adc.h>
#include <delays.h>
#include <stdlib.h>
#include <p18f1330.h>

//Define outputs
#define OUT 2

//Function prototypes
unsigned int getFeedback(unsigned char);

//Which controller am I? (Should be set in hardware)
unsigned char ID = 0x00;

void main(){
	unsigned int position[OUT]; //array of current positions
	unsigned int fb;

	while(1){
		for(fb = 0; fb <= (OUT-1); fb++){
			position[fb] = getFeedback(fb);	
				}																		
			}
}

unsigned int getFeedback(unsigned char ch){

	unsigned int result;
		
	if(ch == 0)	{		//read AN 3
		OpenADC(ADC_FOSC_8 &
			ADC_RIGHT_JUST &
			ADC_0_TAD,
			ADC_CH4 &
			ADC_INT_OFF,
			10);	//bits 0 through 3 of ADCON1, portconfig
		}
	else if(ch == 1) {	//read AN2
		OpenADC(ADC_FOSC_32 &
			ADC_RIGHT_JUST &
			ADC_0_TAD,
			ADC_CH3 &
			ADC_INT_OFF,
			10);	
		}
	Delay10TCYx(5);			//50 Tcy delay
	ConvertADC();			//do work
	while( BusyADC() ); 	//wait
	
	result = ReadADC();
	CloseADC();

	return result;
}
*/

