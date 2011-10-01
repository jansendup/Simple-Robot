#include "p30F4011.h"
#include "uart.h"
#include "system.h"
#include "io_ports.h"

#define BAUD_RATE 57600
#define BAUD_GEN  FCY/(16*BAUD_RATE) - 1     // Nice and rounded

#define RX_BUF_SIZE 16
char rx_buf[RX_BUF_SIZE];
int rx_buf_index = 0;

char* main_commands[] = 
{
    "7SEG",
    "LCD"
};

void main_parser(char terminated);
void (*current_parser)(char terminated) = main_parser;

void _7seg_parse(char terminated);
void _7seg_set_parse(char terminated);

void init_uart()
{
    U1STA   = 0x0000;    // UT1EN = 0 and URXISEL = 0b00
    U1BRG   = BAUD_GEN;
    U1MODE  = 0x8000;    // Enable with: 8-bit, no paritty, 1 stop bit.
    _U1RXIF = 0;
    _U1RXIE = 1;
    _U1RXIP = 2;
    _U1TXIE = 0;
    _UTXEN  = 1;
}

void write_str_uart(char* str)
{
    while( (*str) != '\0' ){
        while (_UTXBF == 1);
        write_uart(*str);
        //while(U1STAbits.TRMT == 0);
        str++;
    }
}


void __attribute__((__interrupt__)) _U1RXInterrupt()
{
    static char escape = 0;
    char recieve;
    _U1RXIF  = 0;
    recieve = U1RXREG;
    
    if(escape)
    {
        escape = 0;
        rx_buf[rx_buf_index] = recieve;
        if(rx_buf_index < RX_BUF_SIZE)
            rx_buf_index++;
        return;
    }
    else
    {
        switch(recieve)
        {
            case '~':
                rx_buf_index = 0;
                current_parser = main_parser;
            break;
            
            case ' ':
                current_parser(0);
            case '#':
                current_parser(1);
            break;
            
            case '\\':
                escape = 1;
            break;
            
            default:
                rx_buf[rx_buf_index] = recieve;
                if(rx_buf_index < RX_BUF_SIZE)
                    rx_buf_index++;
            break;
        }
    }
    
    
    
    if(recieve >= '0' && recieve <= '9')
    {
        write_sreg1(_7SEG_1_MASK,recieve - '0');
        write_sreg1(_7SEG_2_MASK,(9-(recieve - '0'))<<4);
    }
}

void main_parser(char terminated)
{
    int i;
    char match[sizeof(main_commands)];
    for(i = 0; i < sizeof(main_commands); i++)
    {
        match[i] = 1;
    }
    for(i = 0; i < rx_buf_index; i++)
    {
        
    }
}

void _7seg_parse(char terminated)
{
}

void _7seg_set_parse(char terminated)
{
}
