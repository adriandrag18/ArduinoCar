#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <AFMotor.h>

extern void runMotorWithSpeed(AF_DCMotor *motor, int motor_command, int motor_speed);
extern void forward(int my_speed);
extern void backard(int my_speed);
extern void left(int my_speed);
extern void right(int my_speed);
extern void forwardRight(int my_speed);
extern void forwardLeft(int my_speed);
extern void backwardRight(int my_speed);
extern void backwardLeft(int my_speed);
extern void Stop();

#endif