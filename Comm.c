#include "spi.h"
#include <p18f46k22.h>

#define DAC_B		0X8000	//bit masks for setting config bits
#define	DAC_A		0X0000	//in 16-bit DAC string.
#define DAC_B_OFF	0X9000
#define DAC_A_OFF	0X1000


void commandOut(int, unsigned char);			//output command signal
void getRefdata(void);		//read in 10-bit position data, gains


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

void main(){
	commandOut(1023, 15);
}

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
	
	goWhere = inData >> 12;
	//position[goWhere] = inData;
}





			

	

	
	
