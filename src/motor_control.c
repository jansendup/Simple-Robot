#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"
#include "motor_control.h"
#include "quad_encoder.h"

#define D 74L
#define R 10L

#define K_SHIFT 9
#define Kp ((signed long)(5.231*(1<<K_SHIFT)))
#define Ki ((signed long)(1.231*(1<<K_SHIFT)))

#define COUNT_SCALE (signed long)( R*6.2831853*FCY / (ENC_TICKS_PER_REV*4*64) )
#define FP_SHIFT 5

signed long w = 0;
signed long speed = 0;

signed long v1 = 0;
signed long v2 = 0;

signed long v1ei = 0;
signed long v2ei = 0;

signed int dc1 = 0;
signed int dc2 = 0;

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
	PWMCON1bits.PMOD3 = 0;	// PWM I/O pin pair is in the independent output mode
	PWMCON1bits.PEN1H = 1;	// PWM1H pin is enabled for PWM output
	PWMCON1bits.PEN1L = 0;	// PWM1L pin is disabled for PWM output
	PWMCON1bits.PEN2H = 1;	// PWM2H pin is enabled for PWM output
	PWMCON1bits.PEN2L = 0;	// PWM2L pin is disabled for PWM output

	PWMCON1bits.PEN3H = 0;	// PWM3H pin is disabled for PWM output
	PWMCON1bits.PEN3L = 0;	// PWM3L pin is disabled for PWM output

    TMR4 = 0;
    T4CON = 0x8020; // On and prescale divide by 64

	PDC1 = 0;
	PDC2 = 0;
}

void update_motors(void)
{
    signed long delta_count;
    signed long _v1,_v2,v1ep,v2ep;
    signed int d1,d2;
    signed int corr1,corr2;
    unsigned int catch1, catch2;
    
    catch1 = ENC_CNT1;
    catch2 = ENC_CNT2;
    ENC_CNT1 = ENC_COUNT_CENTER;
    ENC_CNT2 = ENC_COUNT_CENTER;
    delta_count = (signed long)TMR4;
    TMR4 = 0;
    
    d1 = catch1 - ENC_COUNT_CENTER;
    d2 = catch2 - ENC_COUNT_CENTER;
    _v1 = (d1)*(COUNT_SCALE << FP_SHIFT) / delta_count ;
    _v2 = (d2)*(COUNT_SCALE << FP_SHIFT) / delta_count ;
    
    v1ep = _v1 - v1;
    v2ep = _v2 - v2;
    
    // Calculate correction:
    corr1 = (Kp*v1ep + Ki*v1ei) >> K_SHIFT;
    corr2 = (Kp*v2ep + Ki*v2ei) >> K_SHIFT;
    
    dc1 += corr1;
    dc2 += corr2;
    
    if(dc1 >= 0)
    {
        MOTOR1_DIRECTION = 1;
        if(dc1 <= PWM_DC_MAX)
        {
            PWM_SET_DC(1,dc1);
        }
        else
        {
            PWM_SET_DC(1,PWM_DC_MAX);
        }
    }
    else
    {
        MOTOR1_DIRECTION = 0;
        if(dc1 >= -PWM_DC_MAX)
        {
            PWM_SET_DC(1,-dc1);
        }
        else
        {
            PWM_SET_DC(1,PWM_DC_MAX);
        }
    }
    
    if(dc2 >= 0)
    {
        MOTOR2_DIRECTION = 1;
        if(dc2 <= PWM_DC_MAX)
        {
            PWM_SET_DC(2,dc2);
        }
        else
        {
            PWM_SET_DC(2,PWM_DC_MAX);
        }
    }
    else
    {
        MOTOR2_DIRECTION = 0;
        if(dc2 >= -PWM_DC_MAX)
        {
            PWM_SET_DC(2,-dc2);
        }
        else
        {
            PWM_SET_DC(2,PWM_DC_MAX);
        }
    }
    
    v1ei += v1ep;
    v2ei += v2ep;
    
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

/* Speed in mm/s */
void set_speed(signed long _speed)
{
    speed = _speed << FP_SHIFT;
    RECALC();
}

/* Speed in mrad/s */
void set_angular_vel(signed long _w)
{
    w = _w << FP_SHIFT;
    RECALC();
}
