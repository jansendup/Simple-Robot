#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define PWM_FREQ 25000
#define PWM_PRESCALE 1
#define PWM_PER ( FCY / (PWM_FREQ * PWM_PRESCALE) - 1 )
#define PWM_DC_MAX (PWM_PER*2)

#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2
#define PWM_SET_DC(MOTOR, DUTY_CYCLE) PDC##MOTOR = DUTY_CYCLE;

#define FORWARD 1
#define REVERSE 0

void init_mt_control(void);
void update_motors(void);
void motor1_feedback(int current);
void motor2_feedback(int current);

/* Speed in 0.25mm/s */
void set_speed(long speed);
/* Speed in 0.25mrad/s */
void set_angular_vel(long w);

#endif
