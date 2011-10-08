#include "std_lib.h"

void int_to_str(char* str, int num)
{
    // TODO: Implement
}

int str_to_int(char* str)
{
    int num = 0;
    char c = *str;
    while(c != '\0' && c >= '0' && c<= '9')
    {
        c = *str;
        num *= 10;
        num += c-'0';
    }
    return num;
}
