#include "p30F4011.h"
#include "system.h"
#include "ad_converter.h"
#include "motor_control.h"

#define TAD_MIN             153.85E-9				// See Table 17-9 on p449 of the referance manual. (500ksps)
#define ADCS_VAL           (int)(2*TAD_MIN*FCY - 1)	// See Equation 17-1 on p413 of the referance manual.

#define AD_PORT_COUNT       7
#define SAMPLES_PER_PORT    1
#define AD_BUFFER_SIZE     (AD_PORT_COUNT * SAMPLES_PER_PORT)
#define INTERRUPT_INTERVAL  AD_BUFFER_SIZE
#define SAMPLE_CYCLES       1

#define MOTOR_1_SENSE       0
#define MOTOR_2_SENSE       1
#define IR_SENSOR_1         2
#define IR_SENSOR_2         3
#define IR_SENSOR_3         4
#define IR_SENSOR_4         5
#define IR_SENSOR_5         6

/*The ADCSSL register specifies the inputs to  be scanned. Each bit in the ADCSSL register
corresponds to an analog input. Bit 0 corresponds to AN0, bit 1 corresponds to AN1 and so on.
If a particular bit in the ADCSSL register is '1', the corresponding input is part of the scan
sequence. 
The inputs are always scanned from lower to higher numbered inputs, starting at the
first selected channel after each interrupt occurs.*/
#define CSSL_VAL   0x1CF
#define PCFG_VAL  (~CSSL_VAL)

volatile int analog_data[AD_BUFFER_SIZE];
volatile char new_analog_data;

void init_ad_converter(void)
{
	// Select voltage reference source to match expected range on analog inputs ADCON2.VCFG: Voltage Reference Configuration bits
	ADCON2bits.VCFG = 0;	// 000: VrefH = AVDD VrefL = AVSS.

	// Select the analog conversion clock to match desired data rate with processor clock ADCON3.ADCS<5:0>: A/D Conversion Clock Select bits
	ADCON3bits.ADRC = 0;	// 0: Use system clock.
	_ADCS = ADCS_VAL;		// Setup TAD.
	
	// Determine how many S/H channels will be used ADCON2<9:8> and ADPCFG<15:0>
	ADCON2bits.CHPS = 0;	//00 = Converts CH0.
	ADCON2bits.ALTS = 0;	//0 = Always use MUX A input multiplexer settings.
	ADCON2bits.CSCNA = 1;	//1 = Scan inputs.
	
	// Determine how sampling will occur ADCON1.SIMSAM and ADCSSL
	ADCON1bits.SIMSAM = 0;	// Dissable Simultaneous Sampling
	ADCSSL = CSSL_VAL;		// Select Ports to form part of sample sequence.

	// Select port pins as analog inputs ADPCFG<15:0>, ADPCFG<0> = port0, ADPCFG<1> = port1 ...
	ADPCFG = PCFG_VAL;	// 1 = Analog input pin in Digital mode, port read input enabled, A/D input multiplexer input connected to AVSS
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

void process_analog_inputs()
{
    char i,k;
    int *value;
	value = analog_data;

    /*TODO: Process analog input data.*/
    for( i = 0,k = 0; i < AD_BUFFER_SIZE; i++,k++,value++)
    {
       switch(k)
       {
          case MOTOR_1_SENSE:
             motor1_feedback(*value);
          break;
          case MOTOR_2_SENSE:
             motor2_feedback(*value);
          break;
          case IR_SENSOR_1:
          break;
          case IR_SENSOR_2:
          break;
          case IR_SENSOR_3:
          break;
          case IR_SENSOR_4:
          break;
          case IR_SENSOR_5:
          break;
       }
       #if SAMPLES_PER_PORT>1
       if(k == AD_PORT_COUNT){
          k -= AD_PORT_COUNT;
       }
       #endif
    }
	new_analog_data = 0;
}

void __attribute__((__interrupt__)) _ADCInterrupt(void)
{
	int *an_data, *buf_data;
	int i = 0;
	an_data = analog_data;
	buf_data = &ADCBUF0;
	
	for (i = 0; i < AD_BUFFER_SIZE; i++,an_data++,buf_data++){
		*analog_data = *buf_data;
	}

	_ADIF = 0;	// Clear AD interrupt flag.
	new_analog_data = 1;
	
}
