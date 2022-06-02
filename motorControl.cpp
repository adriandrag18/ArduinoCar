#include "motorControl.h"

float speed_factor_diagonal_move = 0.5;
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

void runMotorWithSpeed(AF_DCMotor *motor, int motor_command, int motor_speed) {
  motor->setSpeed(motor_speed);
  motor->run(motor_command);
}

void forward(int my_speed)
{
  runMotorWithSpeed(&motor1, FORWARD, my_speed);
  runMotorWithSpeed(&motor2, FORWARD, my_speed);
  runMotorWithSpeed(&motor3, FORWARD, my_speed);
  runMotorWithSpeed(&motor4, FORWARD, my_speed);
}

void backard(int my_speed)
{
  runMotorWithSpeed(&motor1, BACKWARD, my_speed);
  runMotorWithSpeed(&motor2, BACKWARD, my_speed);
  runMotorWithSpeed(&motor3, BACKWARD, my_speed);
  runMotorWithSpeed(&motor4, BACKWARD, my_speed);
}

void left(int my_speed)
{
  runMotorWithSpeed(&motor1, BACKWARD, my_speed);
  runMotorWithSpeed(&motor2, BACKWARD, my_speed);
  runMotorWithSpeed(&motor3, FORWARD, my_speed);
  runMotorWithSpeed(&motor4, FORWARD, my_speed);
}

void right(int my_speed)
{
  runMotorWithSpeed(&motor1, FORWARD, my_speed);
  runMotorWithSpeed(&motor2, FORWARD, my_speed);
  runMotorWithSpeed(&motor3, BACKWARD, my_speed);
  runMotorWithSpeed(&motor4, BACKWARD, my_speed);
}

void forwardRight(int my_speed) {
  runMotorWithSpeed(&motor1, FORWARD, my_speed);
  runMotorWithSpeed(&motor2, FORWARD, my_speed);
  runMotorWithSpeed(&motor3, FORWARD, my_speed * speed_factor_diagonal_move);
  runMotorWithSpeed(&motor4, BACKWARD, my_speed * speed_factor_diagonal_move);
}

void forwardLeft(int my_speed) {
  runMotorWithSpeed(&motor1, FORWARD, my_speed * speed_factor_diagonal_move);
  runMotorWithSpeed(&motor2, BACKWARD, my_speed * speed_factor_diagonal_move);
  runMotorWithSpeed(&motor3, FORWARD, my_speed);
  runMotorWithSpeed(&motor4, FORWARD, my_speed);
}

void backwardRight(int my_speed) {
  runMotorWithSpeed(&motor1, BACKWARD, my_speed);
  runMotorWithSpeed(&motor2, BACKWARD, my_speed);
  runMotorWithSpeed(&motor3, BACKWARD, my_speed * speed_factor_diagonal_move);
  runMotorWithSpeed(&motor4, FORWARD, my_speed * speed_factor_diagonal_move);
}

void backwardLeft(int my_speed) {
  runMotorWithSpeed(&motor1, BACKWARD, my_speed * speed_factor_diagonal_move);
  runMotorWithSpeed(&motor2, FORWARD, my_speed * speed_factor_diagonal_move);
  runMotorWithSpeed(&motor3, BACKWARD, my_speed);
  runMotorWithSpeed(&motor4, BACKWARD, my_speed);
}

void Stop()
{  
  runMotorWithSpeed(&motor1, RELEASE, 0);
  runMotorWithSpeed(&motor2, RELEASE, 0);
  runMotorWithSpeed(&motor3, RELEASE, 0);
  runMotorWithSpeed(&motor4, RELEASE, 0);
}
