#include "p30F4011.h"
#include "io_ports.h"

void init_io_ports()
{
    DEBUG_LED_GREEN_DIR = 0;  // Set direction as output
    DEBUG_LED_GREEN = 1;      // LED OFF ( Active low )
}
