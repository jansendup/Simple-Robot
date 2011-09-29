#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"
#include "ultra_sound.h"
#include "motor_control.h"
#include "ad_converter.h"
#include "quad_encoder.h"
#include "lcd.h"

void flash_led_test();
void init();

extern volatile char new_analog_data;
extern volatile char ultra_sound_idle;

void init()
{
	init_io_ports();
    init_lcd();
	init_quad_encoder();
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
        DEBUG_LED = !DEBUG_LED;
		if((i++%2) == 0){
			//bit_set_reg2(LCD_BL);
            
			//PWM_SET_SPEED(1,50);
            //lcd_character_write('H');
		}
		else{
			//PWM_SET_SPEED(1,80);
            //lcd_scrn_clear();
			//bit_clear_reg2(LCD_BL);
            //write_reg1(0xFFFF,0);
		}
        write_reg1(~(1<<(i%16)),0);
        delay_ms(250);
    }
}
