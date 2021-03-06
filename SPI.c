/*
	======Communications Code======
	configSPI() 	::	Sets control registers for
						SSP1:	SPI Master Mode, for DACs
						SSP2:	SPI Slave Mode, to receive info
					
						Also configures pins for SPI use.
	
	sendtoDAC()		::	Takes and sends a 16-bit int to the
						selected DAC.
						Assumes a mux is connected to PORTA.
	
	commandOut()	::	Takes desired voltage level and a selected
						DAC and performs the logic needed to change
						the sign of the output voltage.
	
	getRefdata()	::	Receives 16-bit data via SSP2 from the Arduino.
						This data should hold a 10-bit value as well
						as some control bits. The upper 6 bits should
						say what the data is (gain/position), and what
						to	do (start/stop/run test?).
	
	
	Derek Sorenson 4/30/11
	
	CHANGELOG:
	v0.02 - Replaced all library function calls with register stuff.
	v0.03 - Changed direction logical "if" statements in commandOut.  
	
*/
#include <p18f46k22.h>
#include "rosefloat.h"
/*CONFIG CONSTANTS*/
#define DAC_B		0XB000	//bit masks for setting config bits
#define	DAC_A		0X3000	//in 16-bit DAC string.
#define DAC_B_OFF	0X8000
#define DAC_A_OFF	0X0000




void configSPI(){
	
	/*
	Setting config registers SSPx for SPI. 
	Call this function from the	setup() function
	*/
	
	//SSP1 : SPI Master Mode for DACs
	SSP1STATbits.SMP = 1;		//Input data sampled at end of data output time
	SSP1STATbits.CKE = 0;		//Transmit occurs on idle->active transition
	
	SSP1CON1bits.SSPEN = 1;	//Serial port enable
	SSP1CON1bits.SSPM = 0b0001;  //SPI frequency = Fosc / 16
	SSP1CON1bits.CKP = 1;

	//SSP2 : SPI Slave Mode (~SS Enabled) for receiving position data
	SSP2STATbits.SMP = 0b0;
	SSP2STATbits.CKE = 0b0;
	SSP2CON1bits.SSPEN = 1;
	SSP2CON1bits.SSPM = 0b0100;

	
	/*PORT DIRECTIONS FOR SPI TO WORK*/
	TRISCbits.RC3 = 0;	//SCK1
	TRISCbits.RC4 = 1;	//SDI1
	TRISCbits.RC5 = 0;	//SDO1
	
	TRISDbits.RD0 = 1;	//SCK2
	TRISDbits.RD1 = 1;	//SDI2
	TRISDbits.RD3 = 1;	//SS 2
	TRISDbits.RD4 = 0;	//SD02

   /*PORT DIRECTIONS FOR CHIP SELECT*/
	TRISAbits.RA6 = 0;	//B0
	TRISAbits.RA7 = 0;	//B1
	TRISCbits.RC0 = 0;	//B2
	TRISCbits.RC1 = 0;	//B3


}


/*	
	MCP4812 DAC Bits 

	bit		function
	==========================
	15		~A/B channel
	14		-
	13		Gain Select
	12		~Shutdown channel
	11-2	Data
	1		-
	0		-
	===========================

	We'll bring the CS of a specific DAC low, and then load
	SSPBUF twice to send the 16 bits.
*/

void sendtoDAC(unsigned int data, unsigned char DACn){
	
	unsigned char dataL = (data & 0x00FF);
	unsigned char dataH = (data >> 8);	

///////////////////////////???????????/////////////////// <- must correct ports
if(DACn>7) //B3
PORTAbits.RC1 = 1;
else
PORTAbits.RC1 = 0;

if(DACn%8>=4) // B2
PORTAbits.RC0 = 1;
else
PORTAbits.RC0 = 0;

if(DACn%4>=2) //B1
PORTAbits.RA7 = 1;
else
PORTAbits.RA7 = 0;

if(DACn%2 == 1) //B0
PORTAbits.RA6 = 1;
else
PORTAbits.RA6 = 0;


	SSP1BUF = dataH; //configbits <7:4>, 4 MSB <3:0>
	while(!SSP1STATbits.BF);
	SSP1BUF = dataL; //6 LSB <7:2>, don't care <1:0>
	while(!SSP1STATbits.BF);
}
	

void commandOut(int voltage, unsigned char DACn){

	int dacdata = 0x0000;				//16 bit output string
	unsigned int DAC_on, DAC_off;		//config bit mask for DAC

	//What direction are we going in?
	if(voltage >= 0){					
		DAC_on = DAC_A;		//going up, use DAC_A
		DAC_off = DAC_B;
	}
	else if(voltage < 0){
		DAC_on = DAC_B;		//going down, use DAC_B
		DAC_off = DAC_A;
		voltage *= -1;
	}

	voltage = voltage & 0x03FF;	//ensure only 10 bit output
	
	
	dacdata = DAC_on;		//set config bits
	voltage = voltage << 2;			
	dacdata = dacdata | voltage;	//or in voltage data
//	DAC_off = DAC_on ^ 0x9000;		//XOR turns off opposite channel
	
	
	sendtoDAC(DAC_off, DACn);

	sendtoDAC(dacdata, DACn);
	
}

/*	
	Recieve Input
*/
void getRefdata(){
	int inData;
	unsigned char H, L;
	
	while(!SSP2STATbits.BF);
	H = SSP2BUF;
	while(!SSP2STATbits.BF);
	L = SSP2BUF;
	
}





			

	

	
	
