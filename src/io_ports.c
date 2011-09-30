#include "p30F4011.h"
#include "io_ports.h"
#include "system.h"

extern int sreg1;
extern char sreg2;

void init_io_ports()
{
    // Configure as outputs:
    DEBUG_LED_TRIS = 0;
    MOTOR1_DIRECTION_TRIS = 0;
    MOTOR2_DIRECTION_TRIS = 0;
    MOTOR1_ENABLE_TRIS = 0;
    MOTOR2_ENABLE_TRIS = 0;	
    ULTRASOUND_TRANSMIT_TRIS = 0;
    SHIFT_REG_1_CLOCK_TRIS = 0;
    SHIFT_REG_1_DATA_TRIS = 0;
    SHIFT_REG_1_STROBE_TRIS = 0;
    SHIFT_REG_2_CLOCK_TRIS = 0;
    SHIFT_REG_2_DATA_TRIS = 0;
    SHIFT_REG_2_STROBE_TRIS = 0;

    // Configure as inputs:
    QUAD_ENCODE_2_A_TRIS = 1;
    QUAD_ENCODE_2_B_TRIS = 1;
    BUMP_SENSOR_TRIS = 1;

    // Setup initial values:
    DEBUG_LED = 1;        // LED OFF ( Active low )
	MOTOR1_ENABLE = 0;    // Switch motors off
	MOTOR2_ENABLE = 0;
}
