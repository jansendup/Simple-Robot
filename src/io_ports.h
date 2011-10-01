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

#define _7SEG_1A      (1)
#define _7SEG_1B      (1<<1)
#define _7SEG_1C      (1<<2)
#define _7SEG_1D      (1<<3)

#define _7SEG_1A_MASK (~_7SEG_1A)
#define _7SEG_1B_MASK (~_7SEG_1B)
#define _7SEG_1C_MASK (~_7SEG_1C)
#define _7SEG_1D_MASK (~_7SEG_1D)
#define _7SEG_1_MASK  (_7SEG_1A_MASK & _7SEG_1B_MASK & _7SEG_1C_MASK & _7SEG_1D_MASK)

#define _7SEG_2A      (1<<4)
#define _7SEG_2B      (1<<5)
#define _7SEG_2C      (1<<6)
#define _7SEG_2D      (1<<7)

#define _7SEG_2A_MASK (~_7SEG_2A)
#define _7SEG_2B_MASK (~_7SEG_2B)
#define _7SEG_2C_MASK (~_7SEG_2C)
#define _7SEG_2D_MASK (~_7SEG_2D)
#define _7SEG_2_MASK  (_7SEG_2A_MASK & _7SEG_2B_MASK & _7SEG_2C_MASK & _7SEG_2D_MASK)

#define LED1          (1<<8)
#define LED2          (1<<9)
#define LED3          (1<<10)
#define LED4          (1<<11)
#define LED5          (1<<12)
#define LED6          (1<<13)
#define LED7          (1<<14)

#define LED1_MASK     (~LED1)
#define LED2_MASK     (~LED2)
#define LED3_MASK     (~LED3)
#define LED4_MASK     (~LED4)
#define LED5_MASK     (~LED5)
#define LED6_MASK     (~LED6)
#define LED7_MASK     (~LED7)
#define LED_MASK      (LED1_MASK & LED2_MASK & LED3_MASK & LED4_MASK & LED5_MASK & LED6_MASK & LED7_MASK)

#define BUZZER_MASK   (~(1<<15))

#define LCD_DB4        (1)
#define LCD_DB5        (1<<1)
#define LCD_DB6        (1<<2)
#define LCD_DB7        (1<<3)
#define LCD_EN         (1<<4)
#define LCD_RW         (1<<5)
#define LCD_RS         (1<<6)
#define LCD_BL         (1<<7)

#define LCD_DB4_MASK   (~LCD_DB4)
#define LCD_DB5_MASK   (~LCD_DB5)
#define LCD_DB6_MASK   (~LCD_DB6)
#define LCD_DB7_MASK   (~LCD_DB7)
#define LCD_EN_MASK    (~LCD_EN)
#define LCD_RW_MASK    (~LCD_RW)
#define LCD_RS_MASK    (~LCD_RS)
#define LCD_BL_MASK    (~LCD_BL)

#define LCD_NIBBLE_MASK              (LCD_DB4_MASK & LCD_DB5_MASK & LCD_DB6_MASK & LCD_DB7_MASK)
#define LCD_CONTROL_MASK             (LCD_EN_MASK & LCD_RW_MASK & LCD_RS_MASK)

#define bit_set_reg1(BIT)     write_sreg1(~BIT,BIT)
#define bit_set_reg2(BIT)     write_sreg2(~BIT,BIT)
#define bit_clear_reg1(BIT)   write_sreg1(~BIT,0)
#define bit_clear_reg2(BIT)   write_sreg2(~BIT,0)

#define set_7seg1(val)   write_sreg1(_7SEG_1_MASK,val)
#define set_7seg2(val)   write_sreg2(_7SEG_2_MASK,(val)<<4)

#include "system.h"

void init_io_ports(void);

extern void write_sreg1(int mask,int bits);
extern void write_sreg2(char mask,char bits);
extern void set_sreg1(int bits);
extern void set_sreg2(char bits);

#endif
