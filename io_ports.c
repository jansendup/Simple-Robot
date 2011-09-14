#include <p30F4011.h>
#include "io_ports.h"

void init_io_ports()
{
    TEST_LED_DIR = 0;
    TEST_LED = 0;
}
