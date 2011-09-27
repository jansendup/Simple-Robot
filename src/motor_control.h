#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define PWM_FREQ 20000
#define PWM_PRESCALE 1
#define PWM_PER ( FCY / (PWM_FREQ * PWM_PRESCALE) - 1 )
#define PWM_SCALE (PWM_PER/50)

#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2
#define PWM_SET_SPEED(MOTOR, DUTY_CYCLE) PDC##MOTOR = DUTY_CYCLE*PWM_SCALE;

#define FORWARD 1
#define REVERSE 0

void init_pwm(void);

#endif
