#include "buzzer.h"
#include "io_ports.h"
#include "ultra_sound.h"
#include "system.h"

#define PROX_COUNT_FACTOR   (US_TIME_OUT_PERIOD/SYSTEM_TIMER_FRQ)
#define PROX_BUZZ_TIME      100E-3
#define PROX_BUZZ_COUNT     ((unsigned int)(SYSTEM_TIMER_FRQ*PROX_BUZZ_TIME))

unsigned int counter = 0;
volatile char buzz_proximity = 1;

extern volatile char us_obj_detected;
extern volatile int us_obj_distance;

void update_buzzer()
{
    unsigned int cnt_threshold;
    counter++;
    
    if(buzz_proximity)
    {
        if(counter > PROX_BUZZ_COUNT)
        {
            // Turn off buzzer
            bit_clr_reg1(BUZZER);
        }
        if(us_obj_detected)
        {
            cnt_threshold = us_obj_distance/PROX_COUNT_FACTOR;
            if(counter > cnt_threshold)
            {
                // Buzz the buzzer
                bit_set_reg1(BUZZER);
                counter = 0;
            }
        }
    }
}
