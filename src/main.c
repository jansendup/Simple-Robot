#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"
#include "ultra_sound.h"
#include "motor_control.h"
#include "ad_converter.h"
#include "quad_encoder.h"
#include "lcd.h"
#include "uart.h"

void init();
void flash_led_test();

extern volatile char new_analog_data;
extern volatile char ultra_sound_idle;
extern volatile char uart_rx;

void init()
{
	init_io_ports();
	init_uart();
    init_lcd();
	init_quad_encoder();
    init_ad_converter();
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
		if( uart_rx )
		    process_uart();
	}

	return 0;
}

void flash_led_test()
{
	int i = 0;
    while(1){
		if((i++%2) == 0){
    		DEBUG_LED = !DEBUG_LED;
		}
		else{
		}
   
        delay_ms(500);
    }
}
