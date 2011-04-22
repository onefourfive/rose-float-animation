#include <spi.h>

#define DAC_B_ON	0X8000	//bit masks for setting config bits
#define	DAC_A_ON	0X0000	//in 16-bit DAC string.
#define DAC_B_OFF	0X9000
#define DAC_A_OFF	0X1000

#pragma code low_vector 0x18

int useDAC(unsigned int, unsigned char);		//send 10-bits to DAC (voltage, chan)
void offDAC(unsigned char);				//disable port function (for crossover)
unsigned int askBrain(unsigned char);			//read in 10-bit position data, gains


/*	To be implemented: pins on the PIC should be dedicated to reading
	what location it is plugged into. This way we PIC1 knows what to 
	listen for over SPI. 

	device_ID should somehow illustrate what PIC this code is running on.

	***IN SPI SLAVE MODE, WE CAN USE CHIPSELECT PINS. EASY MODE***
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

	dacOut(v_out, DAC_B & ON);
*/

void commandOut(int voltage, unsigned int config){

	int dacdata = 0x0000;	//16 bit output string

	if(oldvoltage >= voltage)	//what direction are we going in
		CONFIG_ON = DAC_A_ON;	//going down, use DAC_A
		CONFIG_OFF = DAC_B_OFF;	//turn off DAC_B
	
	else if(oldvoltage <= voltage)
		CONFIG_ON = DAC_B;	//going up, use DAC_B
		CONFIG_OFF = DAC_A_OFF;	//turn off DAC_A

	else
		voltage = 0x0000;
		oldvoltage = oldvoltage;

	dacdata = voltage & 0xFC00;	//ensure only 10 bit output

	dacdata = config & dacdata;	//set config bits
	dacdata = dacdata << 2;		//shift in voltage data


	openSPI1(SPI_FOSC_16, MODE_00, SMP_END);
	putsSPI1(dacdata);
	while(busySPI1);
	closeSPI1();
}


/*	askBrain opens the 2nd SPI channel, SPI2, in slave mode, and
	sees if there is data available. If there is, it stores the
	incoming data to the location specifed by the pointer it's given.
*/
void askBrain(unsigned char* channel){

	int positionData = 0x0000;	//10 bit data
	openSPI2(SLV_SSON, MODE_00, SMP_END);
	
	if(DataRdySPI2){
		getsSPI2(channel, 0x16);	//takes a pointer. right?
	}
}

	



			

	

	
	
