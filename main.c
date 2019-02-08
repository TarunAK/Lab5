/* Edited by: Hari
	Edidted on: Friday, Feb 8 2019
*/
#include <msp.h>
#include <stdint.h>

void ConfigureLed(){
	//Setting red led as GPIO
		//Port 1
	P1SEL0 &= ~0x01;
	P1SEL1 &= ~0x01;
		//Port 2
	P2SEL0 &= ~0x07;
	P2SEL1 &= ~0x07;
	//Setting Direction
		//Port 1
	P1DIR |= 0x01;
		//Port 2
	P2DIR |= 0x07;
	//Setting intitial state 
		//Port 1 
	P1OUT | = 0x01;
		//Port 2 
	P2OUT &= ~0x07;
}

Void ConfigureTimerA1(){
	

}









int main (){
	WDR_A->CTL = WTD_A_CTL_PW | WTD_A_CTL_HOLD;
	ConfigureLed();
	ConfigureTimerA1();

}
