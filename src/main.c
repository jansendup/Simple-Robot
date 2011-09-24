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
	//init_ad_converter();
	init_pwm();
	init_ultra_sound();
}

int main()
{
	init();
	//flash_led_test();

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
    while(1){
        DEBUG_LED_GREEN = !DEBUG_LED_GREEN;
        _500ms_delay();
    }
}
