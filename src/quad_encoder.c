#include "quad_encoder.h"
#include "p30F4011.h"

#define TICKS_PER_REV  36
#define COUNT_CENTER   (4*TICKS_PER_REV)
#define MAX_COUNT      (2*COUNT_CENTER)

int poscnt2;

void init_quad_encoder(void)
{
   _QEOUT = 1;   // Enable digital filter.
   _QECK = 5;    // 1:64 Clock Divide

   _QEIM = 7;     // x4 mode with position counter reset by match (MAXCNT)
   _POSRES = 0;   // Index Pulse doesn't reset Posistion Counter
   _TQCS = 0;     // Selects internal clock.
   _UPDN_SRC = 1;   // QEB pin State Defines Position Counter Direction
   
   POSCNT = 0;  // Initialize count to zero.
   MAXCNT = MAX_COUNT;
   POSCNT1 = COUNT_CENTER;
   POSCNT2 = COUNT_CENTER;

   /** Initialize External Interrupt 1 & 2 phase A and B of second decoder **/
    _INT1IF = 0;    // Clear External Interrupt 1 Flag Status bit
    _INT1IE = 1;    // Enable External Interrupt 1
    _INT1EP = 1;    // Interrupt on negative edge
    _INT1IP = 2;    // Interupt priority
    
    _INT2IF = 0;    // Clear External Interrupt 2 Flag Status bit
    _INT2IE = 1;    // Enable External Interrupt 2
    _INT2EP = 1;    // Interrupt on negative edge
    _INT2IP = 2;    // Interupt priority
}

void __attribute__((__interrupt__)) _INT1Interrupt(void)
{
	_INT1IF = 0;	// Clear External Interrupt 2 Flag Status bit
}

void __attribute__((__interrupt__)) _INT2Interrupt(void)
{
	_INT2IF = 0;	// Clear External Interrupt 2 Flag Status bit
}
