#include <p30F4011.h>
#include "io_ports.h"

_FOSC(XT_PLL16 & CSW_FSCM_OFF);  // Select primary XT clock source with PLL X16
_FGS(CODE_PROT_OFF);             // Disable Code Protection
_FWDT(WDT_OFF);


void flash_led_test();
void _500ms_delay()
{
    long i;
    //Delay = 0.5 seconds
    //Clock frequency = 117.965 MHz
    //Actual delay = 0.5 seconds = 14745600 cycles
    for(i = 0; i < 7000000L; i++);
}

void main()
{
    init_io_ports();
    flash_led_test();
}

void flash_led_test()
{
    while(1){
        TEST_LED = !TEST_LED;
        _500ms_delay();
    }
}
