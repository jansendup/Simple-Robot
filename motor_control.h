#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define PWM_FREQ 20000
#define PWM_PRESCALE 1
#define PWM_PER ( FCY / (PWM_FREQ * PWM_PRESCALE) - 1 )

void init_pwm(void);

#endif
