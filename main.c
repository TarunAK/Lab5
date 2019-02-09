#include "msp.h"

void configNVIC(void)
{	
	NVIC_SetPriority(TA0_N_IRQn, 2);
	NVIC_ClearPendingIRQ(TA0_N_IRQn);
	NVIC_EnableIRQ(TA0_N_IRQn);
	
	NVIC_SetPriority(TA0_0_IRQn, 2);
	NVIC_ClearPendingIRQ(TA0_0_IRQn);
	NVIC_EnableIRQ(TA0_0_IRQn);
	
	NVIC_SetPriority(TA1_N_IRQn, 2);
	NVIC_ClearPendingIRQ(TA1_N_IRQn);
	NVIC_EnableIRQ(TA1_N_IRQn);
}

void configGlobalInterrupts(void)
{
   __ASM("CPSIE I");  
}

void configLED(void)
{
	P1DIR |= 0x01;
	P2DIR |= 0x07;

	P1DS &= ~0x01;
	P2DS &= ~0x07;

	P1OUT &= ~0x01;
	P2OUT &= ~0x07;
}

void configTimer(void)
{
	TIMER_A0->CTL = MC_1 | ID_1 | TASSEL_1 | TAIE;
	TIMER_A0->CTL &= ~TAIFG;
	TIMER_A0->CCR[0] = 0xFFFF;
	TIMER_A0->CCR[1] = 0x1988; 
	
	TIMER_A0->CTL = CCIE;
	TIMER_A0->CCTL[0] &= ~CCIFG;
	TIMER_A0->CCTL[0] &= ~CCIFG;
	
	TIMER_A1->CTL = MC_3 | ID_1 | TASSEL_1 | TAIE;
	TIMER_A1->CTL &= ~TAIFG;
	TIMER_A1->CCR[0] = 0x7FFF;
	//TIMER_A1->CCR[1] = 0xFFFF;
}

int main(void) 
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Disable watchdog timer
	
	// Port 1 GPIO
	P1SEL0 &= ~0x01;
	P1SEL1 &= ~0x01;
	
	// Port 2 GPIO
	P2SEL0 &= ~0x07;
	P2SEL1 &= ~0x07;
	
	
	configNVIC();
	configGlobalInterrupts();
	configLED();
	configTimer();
	
   while(1)
   {
	   __ASM("WFI"); // Wait for interrupt, else sleep
   }
}

void TA0_0_IRQHandler(void)
{
}

//void TA0_N_IRQHandler(void)
//{
//	if ((TIMER_A0->CTL & TAIFG) == 1)
//	{
//	}
//	TA0CTL &= ~TAIFG;
//	P1OUT ^= 0x01;
//	//TIMER_A0->CCTL0 &= ~TAIFG;
//	//NVIC_ClearPendingIRQ(TA0_N_IRQn); // Clear pending Timer A interrupt 
//}
