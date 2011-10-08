#ifndef UART_H
#define UART_H

#define HOOK_ENQ 0x5
#define HOOK_ESC 0x1B

#define write_uart(data) U1TXREG = data;

void init_uart(void);
void write_str_uart(char* str);
void process_uart(void);

#endif
