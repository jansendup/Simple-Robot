#ifndef IO_PORTS_H
#define IO_PORTS_H

#define PORT_MAP(prt,num) PORT##prt##bits.R##prt##num
#define TRIS_MAP(prt,num) TRIS##prt##bits.TRIS##prt##num

// PIN MAPPINGS
#define DEBUG_LED                PORT_MAP(D,2)    // Active low
#define DEBUG_LED_TRIS           TRIS_MAP(D,2)

#define MOTOR1_DIRECTION         PORT_MAP(E,0)
#define MOTOR1_DIRECTION_TRIS    TRIS_MAP(E,0)

#define MOTOR2_DIRECTION         PORT_MAP(E,2)
#define MOTOR2_DIRECTION_TRIS    TRIS_MAP(E,2)

#define MOTOR1_ENABLE            PORT_MAP(E,4)
#define MOTOR1_ENABLE_TRIS       TRIS_MAP(E,4)

#define MOTOR2_ENABLE            PORT_MAP(E,5)
#define MOTOR2_ENABLE_TRIS       TRIS_MAP(E,5)

#define QUAD_ENCODE_2_A          PORT_MAP(D,0)
#define QUAD_ENCODE_2_A_TRIS     TRIS_MAP(D,0)

#define QUAD_ENCODE_2_B          PORT_MAP(D,1)
#define QUAD_ENCODE_2_B_TRIS     TRIS_MAP(D,1)

#define BUMP_SENSOR              PORT_MAP(F,0)
#define BUMP_SENSOR_TRIS         TRIS_MAP(F,0)

#define ULTRASOUND_TRANSMIT      PORT_MAP(F,6)
#define ULTRASOUND_TRANSMIT_TRIS TRIS_MAP(F,6)

#define SHIFT_REG_1_CLOCK        PORT_MAP(F,4)
#define SHIFT_REG_1_CLOCK_TRIS   TRIS_MAP(F,4)

#define SHIFT_REG_1_DATA         PORT_MAP(F,5)
#define SHIFT_REG_1_DATA_TRIS    TRIS_MAP(F,5)

#define SHIFT_REG_1_STROBE       PORT_MAP(F,1)
#define SHIFT_REG_1_STROBE_TRIS  TRIS_MAP(F,1)

#define SHIFT_REG_2_CLOCK        PORT_MAP(C,13)
#define SHIFT_REG_2_CLOCK_TRIS   TRIS_MAP(C,13)

#define SHIFT_REG_2_DATA         PORT_MAP(C,14)
#define SHIFT_REG_2_DATA_TRIS    TRIS_MAP(C,14)

#define SHIFT_REG_2_STROBE       PORT_MAP(D,3)
#define SHIFT_REG_2_STROBE_TRIS  TRIS_MAP(D,3)

#include "system.h"

void init_io_ports(void);

#endif
