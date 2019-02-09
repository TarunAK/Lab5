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

void ConfigureTimerA0(){
	TA0CTL |= 0x0004;
	TA0CTL |= 0x0152;
	TA0CTL &= ~0x02a5;

	
	TA0CCR0 |= 0x8000;





}

void TA0_0_IRQHandler(void){
	if ((TA0CCTL0 & ~(0xfffe))!= 0 ){
		TA0CCTL0 &= !0x0001;
		P1OUT ^= 0x0001;
	}
}

void TA0_N_IRQHandler(void){
	TA0CTL &= ~0x0001;
	P1OUT ^= 0x0001;
}

void ConfigNVIC(){
	NVIC_SetPriority(TA0_0_IRQn, 2);
	NVIC_ClearPendingIRQ(TA0_0_IRQn);
	NVIC_EnableIRQ(TA0_0_IRQn);

	NVIC_SetPriority(TA0_N_IRQn, 2);
	NVIC_ClearPendingIRQ(TA0_N_IRQn);
	NVIC_EnableIRQ(TA0_N_IRQn);
}


void ConfigGlobalInterrupts(){
	__ASM("CPSIE I");
}



int main (){
	WDR_A->CTL = WTD_A_CTL_PW | WTD_A_CTL_HOLD;
	ConfigureLed();
	ConfigureTimerA0();
	ConfigGlobalInterrupts();

	While(1){
		__ASM("WFI");
	}


}
