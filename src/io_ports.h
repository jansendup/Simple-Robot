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

#define _7SEG_1A_MASK (~1)
#define _7SEG_1B_MASK (~(1<<1))
#define _7SEG_1C_MASK (~(1<<2))
#define _7SEG_1D_MASK (~(1<<3))
#define _7SEG_1_MASK  (_7SEG_1A_MASK & _7SEG_1B_MASK & _7SEG_1C_MASK & _7SEG_1D_MASK)

#define _7SEG_2A_MASK (~(1<<4))
#define _7SEG_2B_MASK (~(1<<5))
#define _7SEG_2C_MASK (~(1<<6))
#define _7SEG_2D_MASK (~(1<<7))
#define _7SEG_2_MASK  (_7SEG_2A_MASK & _7SEG_2B_MASK & _7SEG_2C_MASK & _7SEG_2D_MASK)

#define LED1_MASK     (~(1<<8))
#define LED2_MASK     (~(1<<9))
#define LED3_MASK     (~(1<<10))
#define LED4_MASK     (~(1<<11))
#define LED5_MASK     (~(1<<12))
#define LED6_MASK     (~(1<<13))
#define LED7_MASK     (~(1<<14))
#define LED_MASK      (LED1_MASK & LED2_MASK & LED3_MASK & LED4_MASK & LED5_MASK & LED6_MASK & LED7_MASK)

#define BUZZER_MASK   (~(1<<15))

#define LCD_DB4_MASK   (~1)
#define LCD_DB5_MASK   (~(1<<1))
#define LCD_DB6_MASK   (~(1<<2))
#define LCD_DB7_MASK   (~(1<<3))
#define LCD_EN_MASK    (~(1<<4))
#define LCD_RW_MASK    (~(1<<5))
#define LCD_RS_MASK    (~(1<<6))
#define LCD_BL_MASK    (~(1<<7))

#define LCD_NIBBLE_MASK              (LCD_DB4 & LCD_DB5 & LCD_DB6 & LCD_DB7)
#define LCD_CONTROL_MASK             (LCD_EN & LCD_RW & LCD_RS)
#define LCD_CONTROL_SIGNAL(EN,RW,RS) ( EN*(~LCD_EN_MASK) | RW*(~LCD_RW_MASK) | RS*(~LCD_RS_MASK))

#include "system.h"

void init_io_ports(void);

void write_reg_1(int bits, int mask);
void write_reg_2(char bits, char mask);

#endif
