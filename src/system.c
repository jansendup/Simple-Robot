#include "p30F4011.h"
#include "system.h"

_FOSC( 
	#if PLLMODE == 16 
		XT_PLL16
	#endif
	#if PLLMODE == 8 
		XT_PLL8
	#endif
	#if PLLMODE == 4 
		XT_PLL4
	#endif
	& CSW_FSCM_OFF);  		// Select primary XT clock source with PLL X16\8\4

_FGS(CODE_PROT_OFF);		// Disable Code Protection
_FWDT(WDT_OFF);				// Disable Watchdog Timer

volatile char system_upadte = 0;

void init_system()
{
    PR3   = (FCY/(64*SYSTEM_TIMER_FRQ));
    _T3IE  = 1;
    _T3IP  = 4;
    _T3IF  = 0;
    T3CON = 0x8020;  // On and prescale divide by 64
}

void __attribute__((__interrupt__)) _T3Interrupt(void)
{
    _T3IF  = 0;
    system_upadte = 1;
}
