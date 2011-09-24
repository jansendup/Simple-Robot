#include "p30F4011.h"
#include "system.h"

#define TAD_MIN 153.85E-9				// See Table 17-9 on p449 of the referance manual. (500ksps)
#define ADCS (int)(2*TAD_MIN*FCY - 1)	// See Equation 17-1 on p413 of the referance manual.

#define INTERRUPT_INTERVAL 1
#define SAMPLE_CYCLES 1

void init_ad_converter(void)
{
	// Select voltage reference source to match expected range on analog inputs ADCON2.VCFG: Voltage Reference Configuration bits
	ADCON2bits.VCFG = 0;	// 000: VrefH = AVDD VrefL = AVSS.

	// Select the analog conversion clock to match desired data rate with processor clock ADCON3.ADCS<5:0>: A/D Conversion Clock Select bits
	ADCON3bits.ADRC = 0;	// 0: Use system clock.
	//ADCON3bits.ADCS = 1/*ADCS*/;	// Setup TAD.

	// Determine how many S/H channels will be used ADCON2<9:8> and ADPCFG<15:0>
	ADCON2bits.CHPS = 0;	//00 = Converts CH0.
	ADCON2bits.ALTS = 0;	//0 = Always use MUX A input multiplexer settings.
	ADCON2bits.CSCNA = 1;	//1 = Scan inputs.
	
	// Determine how sampling will occur ADCON1.SIMSAM and ADCSSL
	ADCON1bits.SIMSAM = 0;	// Dissable Simultaneous Sampling
	ADCSSL = 0xFFFF;		// Select Ports to form part of sample sequence.

	// Select port pins as analog inputs ADPCFG<15:0>, ADPCFG<0> = port0, ADPCFG<1> = port1 ...
	ADPCFG = 0x0000; // 1 = Analog input pin in Digital mode, port read input enabled, A/D input multiplexer input connected to AVSS
					 // 0 = Analog input pin in Analog mode, port read input disabled, A/D samples pin voltage

	// Determine how inputs will be allocated to S/H channels ADCHS (A/D Input Select Register)
	ADCHSbits.CH0NA = 0;	// 0 = Channel 0 negative input is VREF-.

	// Select the appropriate sample/conversion sequence ADCON1.SSRC/ASAM and ADCON3.SAMC
	ADCON1bits.ASAM = 1;				// Set A/D to automatically begin sampling a channel whenever a conversion is not active on that channel
	ADCON1bits.SSRC = 0b111;			// Conversion Trigger Source Select bits: Internal counter ends sampling and starts conversion (auto convert)
	ADCON3bits.SAMC = SAMPLE_CYCLES;	// Auto Sample Time bits = SAMC*TAD

	// Select how conversion results are presented in the buffer ADCON1.FORM
	ADCON2bits.BUFM = 0;	// Buffer configured as one 16-word buffer ADCBUF(15...0)
	ADCON1bits.FORM = 0;	// 00 = Integer (DOUT = 0000 dddd dddd dddd)
	
	// Setup interrupt.
	ADCON2bits.SMPI = (INTERRUPT_INTERVAL - 1);	// Interrupts at the completion of conversion for each (SMPI+1) sample/convert sequence
	_ADIF = 0;	// Clear AD interrupt flag.
	_ADIP = 2;	// Interrupt priority
	_ADIE = 1;	// Enable interrupt

	// Turn on A/D module ADCON1.ADON
	ADCON1bits.ADON = 1;
}

void __attribute__((__interrupt__)) _ADCInterrupt(void)
{
	_ADIF = 0;	// Clear AD interrupt flag.
}