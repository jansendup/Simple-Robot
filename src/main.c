#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"
#include "ultra_sound.h"
#include "motor_control.h"
#include "ad_converter.h"
#include "quad_encoder.h"
#include "lcd.h"
#include "uart.h"
#include "_7seg.h"
#include "buzzer.h"

void init();
void system_update(void);

extern volatile char new_analog_data;
extern volatile char ultra_sound_idle;
extern volatile char uart_rx;
extern volatile char system_upadte;

void init()
{
	init_io_ports();
	init_system();
	init_uart();
    //init_lcd();
	init_quad_encoder();
    init_ad_converter();
	init_mt_control();
	init_ultra_sound();
}

int main()
{
	init();

	/** System Loop **/
	while(1)
	{
		if( new_analog_data )
			process_analog_inputs();
		if( ultra_sound_idle )
			process_ultra_sound();
		if( uart_rx )
		    process_uart();
		if( system_upadte )
		    system_update();
	}

	return 0;
}

void system_update()
{
    system_upadte = 0;
    update_motors();
    update_7seg();
    update_buzzer();
    DEBUG_LED = !DEBUG_LED;
}
