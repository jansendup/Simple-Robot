#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"
#include "motor_control.h"
#include "quad_encoder.h"

#define D 84L
#define R 10L

#define COUNT_SCALE (long)( R*6.2831853*FCY*4 / (ENC_TICKS_PER_REV*4*64) )

long w = 0;
long speed = 0;

long v1 = 0;
long v2 = 0;

long v1ri = 0;
long v2ri = 0;

long v1rp = 0;
long v2rp = 0;

extern unsigned int ENC_CNT2;
extern char ENC_DIR2;

void init_mt_control()
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

    TMR4 = 0;
    T4CON = 0x8020; // On and prescale divide by 64

	PDC1 = 0;
	PDC2 = 0;
}

void update_motors(void)
{
    long delta_count;
    long _v1,_v2,_v1r,_v2r,_v1rd,_v2rd;
    int d1,d2;
    int catch1, catch2;
    
    catch1 = ENC_CNT1;
    catch2 = ENC_CNT2;
    ENC_CNT1 = ENC_COUNT_CENTER;
    ENC_CNT2 = ENC_COUNT_CENTER;
    delta_count = (long)TMR4;
    TMR4 = 0;
    
    d1 = catch1 - ENC_COUNT_CENTER;
    d2 = catch2 - ENC_COUNT_CENTER;
    _v1 = (d1)*COUNT_SCALE / delta_count ;
    _v2 = (d2)*COUNT_SCALE / delta_count ;
    
    _v1r = v1 - _v1;
    _v2r = v2 - _v2;
    
    _v1rd = _v1r - v1rp;
    _v2rd = _v2r - v2rp;
    
    // Calculate:
    
    v1ri += _v1r;
    v2ri += _v2r;
    
    v1rp = _v1r;
    v2rp = _v2r;
}

void motor1_feedback(int current)
{
}

void motor2_feedback(int current)
{
}

#define RECALC() \
    v1 = speed - (D/2)*w;\
    v2 = speed + (D/2)*w;

/* Speed in 0.25mm/s */
void set_speed(long _speed)
{
    speed = _speed;
    RECALC();
}

/* Speed in 0.25mrad/s */
void set_angular_vel(long _w)
{
    w = _w;
    RECALC();
}
