#ifndef SYSTEM_H
#define SYSTEM_H

#include "p30F4011.h"

#define XTFREQ          7372800           //On-board Crystal frequency
#define PLLMODE         16                //On-chip PLL setting
#define FCY             (XTFREQ*PLLMODE/4)  //Instruction Cycle Frequency

#endif
