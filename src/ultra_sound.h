#ifndef ULTRA_SOUND_H
#define ULTRA_SOUND_H

#define SPEED_OF_SOUND 330.0
#define CUT_OFF_DISTANCE 1.5
#define US_TIME_OUT (2*CUT_OFF_DISTANCE/SPEED_OF_SOUND)
#define US_TIME_OUT_PERIOD (int)(US_TIME_OUT * FCY / 8.0)
#define US_DISTANCE_TO_COUNTS(dis) (int)(2*dis*FCY/(8.0*SPEED_OF_SOUND))

void init_ultra_sound(void);
void process_ultra_sound(void);

#endif
