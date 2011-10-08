#include "_7seg.h"
#include "io_ports.h"
#include "uart.h"

char disp1 = 0,disp2 = 0;
char disp_sel = 0;
char hooked = 0;

extern int sreg1;

void _7seg_hook(char rx)
{
    switch(rx)
    {
        case HOOK_ENQ:
            hooked = 1;
            disp1 = sreg1&(~_7SEG_1_MASK);
            disp2 = (sreg1&(~_7SEG_2_MASK))>>4;
            disp_sel = 0;
            write_str_uart("\n 7SEG Hooked. \n");
        break;
        case HOOK_ESC:
            hooked = 0;
            write_str_uart("\n 7SEG Released. \n");
        break;
        case '+':
            if(disp_sel)
            {
                if(disp2 < 9)
                    disp2++;
                set_7seg2(disp2);
            }
            else
            {
                if(disp1 < 9)
                    disp1++;
                set_7seg1(disp1);
            }
        break;
        case '-':
            if(disp_sel)
            {
                if(disp2 > 0)
                    disp2--;
                set_7seg2(disp2);
            }
            else
            {
                if(disp1 > 0)
                    disp1--;
                set_7seg1(disp1);
            }
        break;
        case '<':
        case ',':
            disp_sel = 1;
        break;
        case '>':
        case '.':
            disp_sel = 0;
        break;
        default:
            if((rx >= '0') && (rx <= '9'))
            {
                rx -= '0';
                if(disp_sel == 1)
                {
                    disp2 = rx;
                    set_7seg2(disp2);
                }
                else
                {
                    disp1 = rx;
                    set_7seg1(disp1);
                }
            }
        break;
    }
}

void update(void)
{
    
}
