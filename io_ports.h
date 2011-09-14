#ifndef IO_PORTS_H
#define IO_PORTS_H

#define TEST_LED PORTBbits.RB8
#define TEST_LED_DIR TRISBbits.TRISB8

void init_io_ports(void);

#endif
