/*
I tried to do what Bob Roy did, but with libraries.
My clock isn't set, I don't have any formulas to account for
potentiometers, but this code might be a bit more readable?
I dunno. Documentation on the libraries is on Dropbox.

Derek 04/13/11
*/


#include <p18f46K22.h> 
#include <stdio.h>
#include <usart.h>
#include <adc.h>

int ADC_CH[16]={
ADC_CH0,ADC_CH1,ADC_CH2,ADC_CH3,
ADC_CH4,ADC_CH5,ADC_CH6,ADC_CH7,
ADC_CH8,ADC_CH9,ADC_CH10,ADC_CH11,
ADC_CH12,ADC_CH13,ADC_CH14,ADC_CH15};

int Kp[16],Ki[16],Kd[16],new_error[16],old_error[16];

int current_position[16],desired_position[16];

int PID_output[16];

int PID(int,int,int,int,int);

int ADC(int);



void main (void){
	
	int i=0;

	//************PWM******************
	// One while(1) loop is 1 Frame
    while(1){	
	    
		for(i=0;i<16;i++)
		{
		/*
		SPI modules to get Desired Position from Big Chip
		*/
		
		desired_position[i]=SPI SHIT;
		
		current_position[i] = ADC(ADC_CH[i]) ;
		//*** dp = get it from Big Chip via SPI
		
		PID_output[i] = PID(current_position[i], desired_position[i], Kp[i], Ki[i], Kd[i]);
		
		SPI_out_shit (PID_output[i]);
		
		}
		while(new_frame==0);  //30Hz external clock
	}
}

int PID(int cp, int dp, int Kp, int Ki, int Kd)
{
	old_error[i] = new_error[i];
	new_error[i] = dp-cp;
	P_err = new_error[i];
	I_err += old_error[i];
	D_err = new_error - old_error;
	
	return (Kp*P_err + Ki*I_err + Kd*D_err);

}
int ADC( int ADC_CH )
{
// configure A/D convertor
OpenADC( ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_15ANA, ADC_CH & ADC_INT_OFF );
Delay10TCYx( 5 ); // Delay for 50TCY
ConvertADC(); // Start conversion
while( BusyADC() ); // Wait for completion
return ReadADC(); //ReadADC(); // Read result
CloseADC(); // Disable A/D converter
}

/*
BusyADC();

char BusyADC( void );

OpenADC( ADC_FOSC_32 &
ADC_RIGHT_JUST &
ADC_14ANA,
ADC_CH[i] &
ADC_INT_OFF );
*/
///////////////////// Sample ADC w/ ADC lib functions//////////
/*
#include <p18C452.h>
#include <adc.h>
#include <stdlib.h>
#include <delays.h>
int result;
void main( void )
{
// configure A/D convertor
OpenADC( ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_8ANA_0REF,
ADC_CH0 & ADC_INT_OFF );
Delay10TCYx( 5 ); // Delay for 50TCY
ConvertADC(); // Start conversion
while( BusyADC() ); // Wait for completion
result = ReadADC(); // Read result
CloseADC(); // Disable A/D converter
}
*/



//////***********getADC from 341L************
/*
unsigned char getADC(void)
{
	ADCON0bits.GO = 1;           //start conversion
	while(ADCON0bits.DONE == 1); //wait for conversion to be completed
	return ADRESH;               //return the most sign 8bits of result
}

void Init_UART()
{
   OSCCON=0X60;
   OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
}

void Init_ADC(void)
{
	ADCON0=0x01; //select channel AN0, and turn on the ADDC subsystem
	ADCON1=0x1d; //set pin 2 as analog signal, 
	ADCON2=0x29; //Set the bit conversion time (TAD) and acquisition time
}
*/






























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