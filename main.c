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
	//Setting CCR0 and CCR1 value to the value of ACLK frequency to ensure it reached max every second
	TA0CCR0 = 0x8000;
	TA0CCR1 = 0x1555;
	//Configuring CCTL0 and CCTL1
		//CCR0
	TA0CCTL0 &= ~0xc9e3;
	TA0CCTL0 |= 0x3413;
		//CCR1
	TA0CCTL1 &= ~0xc9e3;
	TA0CCTL1 |= 0x3413;
	//Ensuring the clock isnt getting divided further
	TA0EX0 &= ~0x0007;
	//Resetting the timer cause
	TA0CTL |= 0x0004;
	//Configuring CTL
	TA0CTL &= ~0x02e7;
	TA0CTL |= 0x0110; 
}

void ConfigureTimerA1(){
	//Setting CCR0 
	TA1CCR0 = 0x4000;
	//Configuring CCTL0
	TA1CCTL0 &= ~0xcde3;
	TA1CCTL0 |= 0x3014;
	//ensuring the clock isnt getting divided further
	TA1EX0 &= ~0x0007;
	//Ressting CCTL1
	TA1CTL |= 0x0004;
	//Configuring CTL
	TA1CTL &= ~0x02f1;
	TA1CTL |= 0x0102;
}

void TA1_0_IRQHandler(void){
	//clears CCIGF flag and changed led state
	TA1CCTL0 &= ~0x0001;
	ChangeRGB_Led();
}

void TA1_N_IRQHandler(void){
	//clears TAIFG and changes led state
	TA1CTL &= ~0x0001;
	ChangeRGB_Led();
}

void ChangeRGB_Led(){
	static int states[8] = {0x00,0x01,0x02,0x03,0x04, 0x05, 0x06, 0x07};
	static int currstate = 0;
	//increments current led state and counter;	
	currstate = (currstate+1)%8;
	P2OUT = ((P2OUT & ~0x07)|states[currstate]);
	
}

void TA0_0_IRQHandler(void){
	if ((TA0CCTL0 & ~(0xfffe))!= 0 ){
		TA0CCTL0 &= ~0x0001;
		P1OUT ^= 0x0001;
	}else if((TA0IV &= 0x0002) != 0){
		//Clearing the flag and truning off CCR1 for TimerA1 
		TA0CCTL1 &= ~ 0x0011;
		//Changing current RGB Led state and enabling TimerA_1
		ChangeRGB_Led();
		TA1CTL |= 0x0030;
	}
}

// void TA0_N_IRQHandler(void){
// 	TA0CTL &= ~0x0001;
// 	P1OUT ^= 0x0001;
// }

void ConfigNVIC(){
	NVIC_SetPriority(TA0_0_IRQn, 2);
	NVIC_ClearPendingIRQ(TA0_0_IRQn);
	NVIC_EnableIRQ(TA0_0_IRQn);

	// NVIC_SetPriority(TA0_N_IRQn, 2);
	// NVIC_ClearPendingIRQ(TA0_N_IRQn);
	// NVIC_EnableIRQ(TA0_N_IRQn);

	NVIC_SetPriority(TA1_0_IRQn, 2);
	NVIC_ClearPendingIRQ(TA1_0_IRQn);
	NVIC_EnableIRQ(TA1_0_IRQn);

	NVIC_SetPriority(TA1_N_IRQn, 2);
	NVIC_ClearPendingIRQ(TA1_N_IRQn);
	NVIC_EnableIRQ(TA1_N_IRQn);

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
