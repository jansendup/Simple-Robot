#ifndef _7SEG_H
#define _7SEG_H

#include "io_ports.h"

#define set_7seg1(val)   write_sreg1(_7SEG_1_MASK,val)
#define set_7seg2(val)   write_sreg1(_7SEG_2_MASK,(val)<<4)

void _7seg_hook(char rx);
void update_7seg(void);

#endif
