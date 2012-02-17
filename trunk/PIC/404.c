#include <p18f4321.h>
#include <pwm.h>

//#pragma config OSC = INTI02
//#pragma config WDT = OFF
//#pragma config MCLRE = ON

void main(){
	
	char period = 0xAA;
		
	Initialize_ADC();
	OpenPWM1(period);		//PWM on pin 17
	SetDCPWM1(Get_ADC());	//ADC on pin 2
}

	


int Get_ADC(void)
{

		ADCON0bits.GO = 1; 			// Start conversion
        while(ADCON0bits.GO==1); 	// Wait for completion
        ADCON0bits.ADON = 0; 		// Disable A/D converter
        return (((int)ADRESH << 8) + ADRESL); //ReadADC(); // Read result

}

void Initialize_ADC(void)       // configure A/D convertor
{
        ADCON0bits.CHS = 0b0000;
        ADCON0bits.ADON = 0b0;

    	ADCON1bits.VCFG1 = 0b00;		//ADC referenced to
        ADCON1bits.VCFG0 = 0b00;		//Vss and Vdd
        ADCON1bits.PCFG  = 0b1110;		//AN0 (pin2) only ADC pin

        ADCON2bits.ADFM = 0b1;    		//RIGHT JUSTIFY
        ADCON2bits.ACQT = 0b011;        //16Tad
        ADCON2bits.ADCS = 0b101;        //Fosc/16
		
}
