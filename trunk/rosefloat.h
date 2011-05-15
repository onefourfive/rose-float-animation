#ifndef ROSEFLOAT_H
#define ROSEFLOAT_H


/*	A	D	C		FUNCTION	PROTOTYPES	*/
/*	S	E	A	N	C	O	N	A	N	T 	*/

/********************************************************************
* Function: Initialize_ADC       
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/
extern void Initialize_ADC(void);

/********************************************************************
* Function: Get_ADC      
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/

int Get_ADC(int);

/********************************************************************
* Function: PID       
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/

int PID(void);

/*	S	P	I		P	R	O	T	O	T	Y	P	E	S*/
/*	D	E	R	E	K		S	O	R	E	N	S	O	N*/

/********************************************************************
* Function:   configSPI     
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/

extern void configSPI(void);

/********************************************************************
* Function:   sendtoDAC     
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/

extern void sendtoDAC(unsigned int, unsigned char);

/********************************************************************
* Function:   commandOut
* 
* PreCondition: 	None
*
* Input:        	Voltage, Channel
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        	Handles control logic based on incoming 10-bit
*					value, then calls sendtoDAC to write out to SPI2.
*
* Note:        		None
*******************************************************************/

extern void commandOut(int, unsigned char);

/********************************************************************
* Function:   getRefdata
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        	Reads in 10-bit position data and gains received
*					over SPI1.
*
* Note:        		Not done. Needs to save data somewhere
*******************************************************************/

extern void getRefdata(void);

/*	I	N	T	E	R	R	U	P	T		S	T	U	F	F	*/
/*	D	E	R	E	K		W	A	S		H	E	R	E	.	*/

/********************************************************************
* Function: configInterrupts      
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/

extern void configInterrupts(void);

/********************************************************************
* Function: configTimer0       
* 
* PreCondition: 	None
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        
*
* Note:        		None
*******************************************************************/

extern void configTimer0(void);

/********************************************************************
* Function: chk_isr      
* 
* PreCondition: 	Interrupt
*
* Input:        	None
*
* Output:     		None
*
* Side Effects: 	None
*
* Overview:        	Checks flags one by one and executes appropriate 
					function in response.
*
* Note:        		None
*******************************************************************/

extern void chk_isr(void);



#endif