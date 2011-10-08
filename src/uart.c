#include "p30F4011.h"
#include "uart.h"
#include "system.h"
#include "io_ports.h"
#include "_7seg.h"
#include "ad_converter.h"
#include "motor_control.h"
#include "std_lib.h"

#define HOOKIT(func)    hook = 1; \
                        hook_func = func;\
                        hook_func(HOOK_ENQ);

#define BAUD_RATE 57600
#define BAUD_GEN  FCY/(16*BAUD_RATE) - 1     // Nice and rounded

#define MAX_ARG_LENGTH 10

#define _7SEG_SRC  0
#define _7SEG_HOOK 1
#define _7SEG_VAL  2

#define A2D_GET    3
#define A2D_STREAM 4

#define LCD_CLR  5
#define LCD_HOOK 6
#define LCD_SHFT 7
#define LCD_SRC  8
#define LCD_W    9

#define MT_DC    10
#define MT_DIR   11
#define MT_HOOK  12
#define MT_ON    13
#define MT_OFF   14

#define CMD_COUNT 15
char* cmds[] =
{
    "7SEG_SRC","7SEG_HOOK","7SEG_VAL",
    "A2D_GET","A2D_STREAM",
    "LCD_CLR","LCD_HOOK","LCD_SHFT","LCD_SRC","LCD_W",
    "MT_DC","MT_DIR","MT_HOOK","MT_ON","MT_OFF"
};

char rx_match_index = 0;
char rx_matching_cmds[CMD_COUNT];
char args[MAX_ARG_LENGTH];
char args_length;
char rx_arguments;

void rx_cmd_char(char rx);
void rx_cmd(int match_index);
void rx_clear();

char hook = 0;
void (*hook_func) (char rx) = 0;

char uart_rx = 0;
char recieve;

void mt_duty_cycle(void);
void mt_on_off(char on_off);

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
    rx_clear();
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
    _U1RXIF  = 0;
    recieve = U1RXREG;
    uart_rx = 1;
    
}

void process_uart()
{
    static char escape = 0;
    uart_rx = 0;
    DEBUG_LED = !DEBUG_LED;
    if(escape)
    {
        escape = 0;
        rx_cmd_char(recieve);
        return;
    }
    else
    {
        switch(recieve)
        {
            case '~':
                rx_clear();
            break;
            case '\\':
                escape = 1;
            break;
            
            default:
                rx_cmd_char(recieve);
            break;
        }
    }
}

void rx_cmd_char(char rx)
{
    int i;
    static int matched_cmd = 0;
    char isSpace = (rx == ' ');
    
    if ( hook )
    {
        if(hook_func)
            hook_func(rx);
        return;
    }
    
    if( isSpace || rx == '#' || rx == '!' || rx == '\n' || rx == 0xD)
    {
        if(rx_arguments)
        {
            if(!isSpace)
            {
                rx_cmd(matched_cmd);
            }
            else
            {
                if(args_length < MAX_ARG_LENGTH){
                    args[args_length] = rx;
                    args_length++;
                }
            }
        }
        else
        {
            for(i = 0; i < CMD_COUNT; i++)
            {
                if( (rx_matching_cmds[i] == 1) && (cmds[i][rx_match_index] == '\0'))
                {
                    if(rx == ' '){
                        rx_arguments = 1;
                        args_length = 0;
                        matched_cmd = i;
                    }    
                    else{
                        rx_cmd(i);
                    }    
                }
                rx_matching_cmds[i] = 1;
            }
            rx_match_index = 0;
        }
        
    }
    else
    {
        if(rx >= 'a' && rx <= 'z')
            rx += 'A'-'a';
        for(i = 0; i < CMD_COUNT; i++)
        {
            if( rx_matching_cmds[i] )
            {
                if((rx != cmds[i][rx_match_index]) || (cmds[i][rx_match_index] == '\0'))
                {
                    rx_matching_cmds[i] = 0;
                }
            }
        }
        rx_match_index++;
    }
}

void rx_cmd(int match_index)
{
    switch(match_index)
    {
        case _7SEG_SRC:
        break;
        case _7SEG_HOOK:
            HOOKIT(_7seg_hook);
        break;
        case _7SEG_VAL:
        break;
        
        case A2D_GET:
        break;
        case A2D_STREAM:
            HOOKIT(ad_stream_hook);
        break;
        
        case LCD_CLR:
        break;
        case LCD_HOOK:
        break;
        case LCD_SHFT:
        break;
        case LCD_SRC:
        break;
        case LCD_W:
        break;
        
        case MT_DC:
            mt_duty_cycle();
            rx_clear();
        break;
        case MT_ON:
            mt_on_off(1);
        break;
        case MT_OFF:
            mt_on_off(0);
        break;
    }
}

void rx_clear()
{
    int i;
    rx_match_index = 0;
    for(i = 0; i < CMD_COUNT; i++)
    {
        rx_matching_cmds[i] = 1;
    }
    rx_arguments = 0;
    args_length = 0;
    if(hook)
    {
        if(hook_func)
            hook_func(HOOK_ESC);
        hook = 0;
    }
    
}

void mt_duty_cycle()
{
    int motor, dutyc;
    char i;
    if(args_length < 3){
        write_str_uart("Bad syntax.");
        return;
    }    
    motor = str_to_int(args);
    if(motor > 2 || motor < 1)
    {
        write_str_uart("Bad syntax.");
        return;
    }
    while(i++ < args_length)
    {
        if(args[i] == ' ')
        {
            dutyc = str_to_int(args+i);
            if(dutyc <= 100)
            {
                if(motor == 1){
                    PWM_SET_SPEED(1,dutyc);
                }    
                else{
                    PWM_SET_SPEED(2,dutyc);
                }    
            }
            else
            {
                write_str_uart("Bad syntax.");
            }
            return;
        }
    }
}

void mt_on_off(char on_off)
{
    int motor;
    char i;
    if(args_length < 1){
        write_str_uart("Bad syntax.");
        return;
    }
    motor = str_to_int(args);
    if(motor > 2 || motor < 1)
    {
        write_str_uart("Bad syntax.");
        return;
    }
    if(motor == 1)
    {
        MOTOR1_ENABLE = on_off;
    }
    else if(motor == 2)
    {
        MOTOR2_ENABLE = on_off;
    }
    else
    {
        write_str_uart("Bad syntax.");
    }
}

