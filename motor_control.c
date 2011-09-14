#include "p30F4011.h"
#include "system.h"

#define MOTOR1_DIRECTION PORTEbits.RE0
#define MOTOR1_DIRECTION_TRIS TRISEbits.TRISE0

#define MOTOR2_DIRECTION PORTEbits.RE2
#define MOTOR2_DIRECTION_TRIS TRISEbits.TRISE2

void init_pwm()
{
	/** Setup PWM Time Base Control Register **/
	PTCONbits.PTEN = 1;		// Enable PWM Counter
	PTCONbits.PTOPS = 0x0;	// Output postscale
	PTCONbits.PTCKPS = 		// Clock input prescale = 4^PTCKPS
						#if PWM_PRESCALE == 16
							0x4;
						#endif
						#if PWM_PRESCALE == 8
							0x3;
						#endif
						#if PWM_PRESCALE == 4
							0x2;
						#endif
						#if PWM_PRESCALE == 2
							0x1;
						#endif
						#if PWM_PRESCALE == 1
							0x0;
						#endif
	PTCONbits.PTMOD = 0x0;	// Set PWM to free running mode.

	/** Setup PWM Period **/
	PTPER = PWM_PER;

	/** Setup PWM Control Register 1 **/
	PWMCON1bits.PMOD1 = 0;	// PWM I/O pin pair is in the independent output mode
	PWMCON1bits.PMOD2 = 0;	// PWM I/O pin pair is in the independent output mode
	PWMCON1bits.PEN1H = 1;	// PWM1H pin is enabled for PWM output
	PWMCON1bits.PEN1L = 0;	// PWM1L pin is disabled for PWM output
	PWMCON1bits.PEN2H = 1;	// PWM2H pin is enabled for PWM output
	PWMCON1bits.PEN2L = 0;	// PWM2L pin is disabled for PWM output

	PDC1 = 0;
	PDC2 = 0;

	/** Setup motor direction control pins **/
	MOTOR1_DIRECTION_TRIS = 0;	// Set pin to output
	MOTOR2_DIRECTION_TRIS = 0;	// Set pin to output
	MOTOR1_DIRECTION = 1;		// Initial direction
	MOTOR2_DIRECTION = 1;		// Initial direction
}
