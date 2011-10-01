#include "ultra_sound.h"
#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"

#define TRANSMIT_ON 1
#define TRANSMIT_OFF 0

#define TRANSMISSION_FREQ 40000
#define TRANSMISSION_CYCLES 8
#define TRANSMISSION_LENGTH (int)(TRANSMISSION_CYCLES * FCY / (8.0*TRANSMISSION_FREQ))
#define OVERHEAD_CORRECTION 10

#define US_TIMER_START() T2CONbits.TON = 1;
#define US_TIMER_STOP() T2CONbits.TON = 0;
#define US_TIMER_RESET() TMR2 = 0;

#define SPEED_OF_SOUND 330.0
#define CUT_OFF_DISTANCE 1.5
#define US_TIME_OUT (2*CUT_OFF_DISTANCE/SPEED_OF_SOUND)
#define US_TIME_OUT_PERIOD (int)(US_TIME_OUT * FCY / 8.0)
#define US_DISTANCE_TO_COUNTS(dis) (int)(2*dis*FCY/(8.0*SPEED_OF_SOUND))

volatile char ultra_sound_idle = 1;
volatile char signal_received = 0;

void init_ultra_sound()
{
	/** Initialize Timer 2 **/
	T2CONbits.TON = 0;		// Stop Timer
	TMR2 = 0;				// Set Timer to zero
	T2CONbits.TCKPS = 0x1;	// Timer Input Clock Prescale = 1:8 (DO NOT CHANGE!)
	T2CONbits.TGATE = 0x0;	// Timer gated time accumulation disabled
	T2CONbits.TCS = 0;		// Selects internal clock FCY as source
	PR2 = US_TIME_OUT_PERIOD;		// Set cut-off timer value.

	_T2IF = 0;		// Clear Timer2 interrupt status flag
	_T2IE = 1;		// Enable timer interupt
	_T2IP = 2;		// Interupt priority
	
	/** Initialize External Interrupt 0 for signal lock input **/
	_INT0IF = 0;	// Clear External Interrupt 0 Flag Status bit
	_INT0IE = 1;	// Enable External Interrupt 0
	_INT0EP = 1;	// Interrupt on negative edge
	_INT0IP = 2;	// Interupt priority
	
}

void process_ultra_sound()
{
	// Process readings
	if( signal_received )
	{
		// TODO: Process readings.
	}
	// Restart the timer.
	US_TIMER_RESET();

	// Transmit a signal for TRANSMISSION_LENGTH timer counts.
	signal_received = 0;
	PR2 = TRANSMISSION_LENGTH;
	ULTRASOUND_TRANSMIT = TRANSMIT_ON;
	US_TIMER_START();

	ultra_sound_idle = 0;
}

// Timer 2 will generate this interrupt when it hasn't been stopped by
// _INT0Interrupt and the timer's value is greater than the value of its
// periode register or if the signal should stop transmitting.
void __attribute__((__interrupt__)) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0;		// Clear Timer2 interrupt status flag
	if(PR2 == TRANSMISSION_LENGTH)
	{
		PR2 = US_TIME_OUT_PERIOD;
		TMR2 = (TRANSMISSION_LENGTH + OVERHEAD_CORRECTION);
		ULTRASOUND_TRANSMIT = TRANSMIT_OFF;
	}
	else
	{
		US_TIMER_STOP();
		ultra_sound_idle = 1;
	}
}

void __attribute__((__interrupt__)) _INT0Interrupt(void)
{
	IFS0bits.INT0IF = 0;	// Clear External Interrupt 2 Flag Status bit
	US_TIMER_STOP();
	signal_received = 1;
	ultra_sound_idle = 1;
}
