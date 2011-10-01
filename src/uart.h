#ifndef UART_H
#define UART_H

#define write_uart(data) U1TXREG = data;

void init_uart(void);
void write_str_uart(char* str);

#endif
