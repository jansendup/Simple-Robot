#include "quad_encoder.h"
#include "p30F4011.h"
#include "io_ports.h"

volatile unsigned int ENC_CNT2;
volatile char ENC_DIR2;

void init_quad_encoder(void)
{
   _QEOUT = 1;   // Enable digital filter.
   _QECK = 5;    // 1:64 Clock Divide

   _QEIM = 7;     // x4 mode with position counter reset by match (MAXCNT)
   _POSRES = 0;   // Index Pulse doesn't reset Posistion Counter
   _TQCS = 0;     // Selects internal clock.
   _UPDN_SRC = 1;   // QEB pin State Defines Position Counter Direction
   
   MAXCNT = ENC_MAX_COUNT;
   ENC_CNT1 = ENC_COUNT_CENTER;
   ENC_CNT2 = ENC_COUNT_CENTER;

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
	_INT1IF = 0;        // Clear External Interrupt 2 Flag Status bit
	_INT1EP = !_INT1EP; // Both edges of interrupt.
	if((QUAD_ENCODE_2_A ^ QUAD_ENCODE_2_B) == 1)
	{
    	ENC_CNT2++;
    	ENC_DIR2 = 1;
	}
	else
	{
    	ENC_CNT2--;
    	ENC_DIR2 = 0;
	}
}

void __attribute__((__interrupt__)) _INT2Interrupt(void)
{
	_INT2IF = 0;        // Clear External Interrupt 2 Flag Status bit
	_INT2EP = !_INT2EP; // Both edges of interrupt.
	if((QUAD_ENCODE_2_A ^ QUAD_ENCODE_2_B) == 0)
	{
    	ENC_CNT2++;
    	ENC_DIR2 = 1;
	}
	else
	{
    	ENC_CNT2--;
    	ENC_DIR2 = 0;
	}
}
