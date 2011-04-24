/*
I tried to do what Bob Roy did, but with libraries.
My clock isn't set, I don't have any formulas to account for
potentiometers, but this code might be a bit more readable?
I dunno. Documentation on the libraries is on Dropbox.

Derek 04/13/11
Sean 4/23/11
*/


#include <p18f46K22.h> 
#include <stdio.h>
#include <adc.h>
#include <delays.h>
#include <spi.h>
#include <pconfig.h>


#define DAC_B		0X8000	//bit masks for setting config bits
#define	DAC_A		0X0000	//in 16-bit DAC string.
#define DAC_B_OFF	0X9000
#define DAC_A_OFF	0X1000

int Kp[16],Ki[16],Kd[16],new_error[16],old_error[16],P_err[16],I_err[16],D_err[16];
int current_position[16],desired_position[16];
int PID_output[16];

unsigned char i=0;

int PID(int,int,int,int,int);
int ADC(int);

void commandOut(int, unsigned char);			//output command signal
void getRefdata(void);		//read in 10-bit position data, gains

void main (void){
	

	//************PWM******************
	// One while(1) loop is 1 Frame
    while(1){	
	    
		for(i=0;i<16;i++)
		{
		/*
		SPI modules to get Desired Position from Big Chip
		*/
		
		getRefdata();
		
		current_position[i] = ADC(i) ;
		//*** dp = get it from Big Chip via SPI
		
		PID_output[i] = PID(current_position[i], desired_position[i], Kp[i], Ki[i], Kd[i]);
		
		commandOut(PID_output[i],i);
		
		}
		//while(new_frame==0);  //30Hz external clock
	}
}

int PID(int cp, int dp, int Kp, int Ki, int Kd)
{
	old_error[i] = new_error[i];
	new_error[i] = dp-cp;
	P_err[i] = new_error[i];
	I_err[i] += old_error[i];
	D_err[i] = new_error - old_error;
	
	return (Kp*P_err[i] + Ki*I_err[i] + Kd*D_err[i]);

}
int ADC( int ADC_CH )
{
// configure A/D convertor
ADCON0 = i*4 + 0x03; //Selects ADChannel "i" and turns on ADC
ADCON1 = 0x1D;
ADCON2 = 0x29;
Delay10TCYx( 5 ); // Delay for 50TCY
ConvertADC(); // Start conversion
while( BusyADC() ); // Wait for completion
return ReadADC(); //ReadADC(); // Read result
CloseADC(); // Disable A/D converter
}


/*	To be implemented: pins on the PIC should be dedicated to reading
	what location it is plugged into. This way we PIC1 knows what to 
	listen for over SPI. 

	device_ID should somehow illustrate what PIC this code is running on.

	***IN SPI SLAVE MODE, WE CAN USE CHIPSELECT PINS. EASY MODE***
	***EXCEPT WHERE THE HELL DO WE GET A BIG-ASS MUX???***
*/



/*	we can use putsSPI() to write strings of any length to the SPI bus.
	in this case we use strings of 16-bit length to send the data the 
	MCP4812 DAC requires.

	bit		function
	==========================
	15		~A/B channel
	14		-
	13		Gain Select
	12		~Shutdown channel
	11-3	Data
	2		-
	1		-
	===========================

	dacOut uses the putsSPI function to write the 16 bits
	of information the MCP4812 requires.

	This function should be fixed: putsSPI requires a pointer
	to a string.

	The char 'config' should be set by using the #define statements
	above: i.e.

	useDAC(v_out, DAC_B & ON);
*/

void commandOut(int voltage, unsigned char DACn){

	int dacdata = 0x0000;				//16 bit output string
	unsigned int DAC_on, DAC_off;				//config bit mask for DAC


	//What direction are we going in?
	if(voltage > 0){					
		DAC_on = DAC_A;		//going up, use DAC_A
	}
	else if(voltage <= 0){
		DAC_on = DAC_B;		//going down, use DAC_B
		voltage *= -1;
	}
	
	if(voltage > 1023)
	{voltage = 1023;}
	voltage = voltage & 0x03FF;	//ensure only 10 bit output

	dacdata = DAC_on;		//set config bits
	voltage = voltage << 2;			
	dacdata = dacdata | voltage;	//shift + or in voltage data
	DAC_off = DAC_on ^ 0x9000;		//XOR turns off opposite channel
	
	PORTD = DACn;			//DACn determines pins on ~CS bus (to mux)
	
	OpenSPI(SPI_FOSC_16, MODE_00, SMPEND);
	putsSPI(dacdata);
	while(busySPI());
	putsSPI1(DAC_off);
	while(busySPI());
	closeSPI();
}

/*	getRefdata opens the 2nd SPI channel, SPI2, in slave mode, and
	sees if there is data available. If there is, it stores the
	incoming data to the location specifed by the pointer it's given.
	
	Should this function be called via an interrupt?
*/
void getRefdata(){

	int inData = 0x0000;	//10 bit data
	int goWhere = 0x0000;	//where to put it
	int* input = &inData;
	openSPI2(SLV_SSON, MODE_00, SMPEND);
	
	if(DataRdySPI2()){
		getsSPI2(input, 0x16);	//takes a pointer. right?
	}
	
	desired_position[i]= inData >> 12;
	//position[goWhere] = inData;
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