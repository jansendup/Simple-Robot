///*
//; � 2005 Microchip Technology Inc.
//;
//; FileName:        traps.c
//; Dependencies:    Header (.h) files if applicable, see below
//; Processor:       dsPIC30F
//; Compiler:        MPLAB� C30 v1.32 or higher
//;
//; SOFTWARE LICENSE AGREEMENT:
//; Microchip Technology Inc. (�Microchip�) licenses this software to you
//; solely for use with Microchip dsPIC� digital signal controller
//; products. The software is owned by Microchip and is protected under
//; applicable copyright laws.  All rights reserved.
//;
//; SOFTWARE IS PROVIDED �AS IS.�  MICROCHIP EXPRESSLY DISCLAIMS ANY
//; WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
//; LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//; PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
//; BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
//; DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
//; PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
//; BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
//; ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
//;
//; REVISION HISTORY:
//;  $Log: traps.c,v $
//;  Revision 1.1.1.1  2005/06/06 09:16:45  VasukiH
//;  First release of software
//;
//;
//; ADDITIONAL NOTES:
//; 1. This file contains trap service routines (handlers) for hardware
//;    exceptions generated by the dsPIC30F device.
//; 2. All trap service routines in this file simply ensure that device
//;    continuously executes code within the trap service routine. Users
//;    may modify the basic framework provided here to suit to the needs
//;    of their application.
//;
//*/

#include "p30F4011.h"

void __attribute__((__interrupt__)) _OscillatorFail(void);
void __attribute__((__interrupt__)) _AddressError(void);
void __attribute__((__interrupt__)) _StackError(void);
void __attribute__((__interrupt__)) _MathError(void);
void __attribute__((__interrupt__)) _AltOscillatorFail(void);
void __attribute__((__interrupt__)) _AltAddressError(void);
void __attribute__((__interrupt__)) _AltStackError(void);
void __attribute__((__interrupt__)) _AltMathError(void);

/*
Primary Exception Vector handlers:
These routines are used if INTCON2bits.ALTIVT = 0.
All trap service routines in this file simply ensure that device
continuously executes code within the trap service routine. Users
may modify the basic framework provided here to suit to the needs
of their application.
*/
void __attribute__((__interrupt__)) _OscillatorFail(void)
{
        INTCON1bits.OSCFAIL = 0;        //Clear the trap flag
        while (1);
}

void __attribute__((__interrupt__)) _AddressError(void)
{
        INTCON1bits.ADDRERR = 0;        //Clear the trap flag
        while (1);
}
void __attribute__((__interrupt__)) _StackError(void)
{
        INTCON1bits.STKERR = 0;         //Clear the trap flag
        while (1);
}

void __attribute__((__interrupt__)) _MathError(void)
{
        INTCON1bits.MATHERR = 0;        //Clear the trap flag
        while (1);
}

/*
Alternate Exception Vector handlers:
These routines are used if INTCON2bits.ALTIVT = 1.
All trap service routines in this file simply ensure that device
continuously executes code within the trap service routine. Users
may modify the basic framework provided here to suit to the needs
of their application.
*/

void __attribute__((__interrupt__)) _AltOscillatorFail(void)
{
        INTCON1bits.OSCFAIL = 0;
        while (1);
}

void __attribute__((__interrupt__)) _AltAddressError(void)
{
        INTCON1bits.ADDRERR = 0;
        while (1);
}

void __attribute__((__interrupt__)) _AltStackError(void)
{
        INTCON1bits.STKERR = 0;
        while (1);
}

void __attribute__((__interrupt__)) _AltMathError(void)
{
        INTCON1bits.MATHERR = 0;
        while (1);
}