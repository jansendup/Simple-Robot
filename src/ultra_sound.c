#include "ultra_sound.h"
#include "p30F4011.h"
#include "system.h"

#define US_TIMER_START() T2CONbits.TON = 1;
#define US_TIMER_STOP() T2CONbits.TON = 0;
#define US_TIMER_RESET() TMR2 = 0;

#define SPEED_OF_SOUND 330.0
#define CUT_OFF_DISTANCE 1.5
#define US_TIME_OUT (2*CUT_OFF_DISTANCE/SPEED_OF_SOUND)
#define US_PERIOD (int)(US_TIME_OUT * FCY / 8.0)	// With prescale = 1:8

void init_ultra_sound()
{
	/** Initialize Timer 2 **/
	T2CONbits.TON = 0;		// Stop Timer
	TMR2 = 0;				// Set Timer to zero
	T2CONbits.TCKPS = 0x1;	// Timer Input Clock Prescale = 1:8
	T2CONbits.TGATE = 0x0;	// Timer gated time accumulation disabled
	T2CONbits.TCS = 0;		// Selects internal clock FCY as source
	PR2 = US_PERIOD;		// Set cut-off timer value.

	_T2IF = 0;		// Clear Timer2 interrupt status flag
	_T2IE = 1;		// Enable timer interupt
	_T2IP = 2;		// Interupt priority
	
	/** Initialize External Interrupt 2 for signal lock input **/
	_INT2IF = 0;	// Clear External Interrupt 2 Flag Status bit
	_INT2IE = 1;	// Enable External Interrupt 2
	_INT2EP = 1;	// Interrupt on negative edge
	_INT2IP = 0x2;	// Interupt priority
}

void __attribute__((__interrupt__)) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0;		// Clear Timer2 interrupt status flag
	US_TIMER_STOP();
	US_TIMER_RESET();
}

void __attribute__((__interrupt__)) _INT2Interrupt(void)
{
	IFS1bits.INT2IF = 0;	// Clear External Interrupt 2 Flag Status bit
}
