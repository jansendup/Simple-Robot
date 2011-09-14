#include "p30F4011.h"
#include "system.h"
#include "io_ports.h"

void flash_led_test();
void _500ms_delay()
{
    long i;
    for(i = 0; i < 7000000L; i++);
}

int main()
{
	init_io_ports();
	flash_led_test();
	return 0;
}

void flash_led_test()
{
    while(1){
        DEBUG_LED_GREEN = !DEBUG_LED_GREEN;
        _500ms_delay();
    }
}