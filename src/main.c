#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"
#include "ultra_sound.h"
#include "motor_control.h"
#include "ad_converter.h"

void flash_led_test();
void init();

extern volatile int analog_data[AD_BUFFER_SIZE];
extern volatile char new_analog_data;

extern volatile char ultra_sound_idle;

void _500ms_delay()
{
    long i;
    for(i = 0; i < 7000000L; i++);
}

void init()
{
	init_io_ports();
	init_ad_converter();
	init_pwm();
	init_ultra_sound();
}

int main()
{
	init();
	flash_led_test();

	/** System Loop **/
	while(1)
	{
		if( new_analog_data )
			process_analog_inputs();
		if( ultra_sound_idle )
			process_ultra_sound();
	}

	return 0;
}

void flash_led_test()
{
	int i = 0;
    while(1){
        DEBUG_LED_GREEN = !DEBUG_LED_GREEN;
		PORTEbits.RE4 = 1;
		if((i++%2) == 0){
			PWM_SET_SPEED(1,50);
		}
		else{
			PWM_SET_SPEED(1,80);
			//PDC1 = PWM_PER;
		}
        _500ms_delay();
    }
}
