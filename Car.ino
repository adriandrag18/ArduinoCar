#include <Servo.h>
#include <NewPing.h>
#include <AFMotor.h>
#include <SoftwareSerial.h>

#define rxPin 4
#define txPin 5

// Set up a new SoftwareSerial object
//SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

#define TRIGGER_PIN  A1  
#define ECHO_PIN     A0  
#define MAX_DISTANCE 250 
#define MAX_SPEED 255
#define SERVO_INIT 90
#define SERVO_LEFT 45
#define SERVO_RIGHT 135
#define CONTROLED 1
#define AUTO 2
#define RETRACE 3
#define MAX_ACTIONS 200

Servo myservo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

typedef struct {
  char command;
  int _speed;
  int duration;  
} action;

float speed_factor_diagonal_move = 0.5;

char command;
int controled_speed = MAX_SPEED / 2;
int auto_speed = MAX_SPEED / 2;
int distance = 0;
int distance_left = 0;
int distance_right = 0;
int mode = CONTROLED;
int threshlod_distance = 25;
int time_to_turn = 900;
int time_step = 30;

action actions[MAX_ACTIONS];
int action_index = 0;

void setup() {
  myservo.attach(9);  // servo2
  Serial.begin(9600);
//  mode = AUTO;
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    decodeCommand(command);
  }
  if (mode == AUTO) {
    autoControl();
  }
}

void decodeCommand(char command)
{
  switch (command) {
    case 'F':
      mode = CONTROLED;
      addAction('B', controled_speed, time_step);
      forward(controled_speed);
      break;
    case 'B':
      mode = CONTROLED;
      addAction('F', controled_speed, time_step);
      backard(controled_speed);
      break;
    case 'L':
      mode = CONTROLED;
      addAction('R', controled_speed, time_step);
      left(controled_speed);
      break;
    case 'R':
      mode = CONTROLED;
      addAction('L', controled_speed, time_step);
      right(controled_speed);
      break;
    case 'G':
      mode = CONTROLED;
      addAction('I', controled_speed, time_step);
      forwardLeft(controled_speed);
      break;
    case 'I':
      mode = CONTROLED;
      addAction('G', controled_speed, time_step);
      forwardRight(controled_speed);
      break;
    case 'H':
      mode = CONTROLED;
      addAction('J', controled_speed, time_step);
      backwardLeft(controled_speed);
      break;
    case 'J':
      mode = CONTROLED;
      addAction('H', controled_speed, time_step);
      backwardRight(controled_speed);
      break;
    case 'S':
      if (mode == CONTROLED)
        Stop();
      break;
    case 'U':
      retrace();
      break;
    case 'W':
      mode = AUTO;
      myservo.write(0);
      delay(500);
      myservo.write(SERVO_INIT);
      delay(500);
      break;
    case 'w':
      mode = CONTROLED;
      break;
    case 'q':
      controled_speed = MAX_SPEED;
      break;
    default:
      if ('0' <= command && command <= '9') {
        int factor = command - '0';
        controled_speed = MAX_SPEED * (10 + factor) / 20;
      }
      break;
  }
}

void addAction(char action_command, int action_speed, int action_duration) {
  if (action_index >= MAX_ACTIONS)
    return;
  action tmp;
  tmp.command = action_command;
  tmp._speed = action_speed;
  tmp.duration = action_duration;
  actions[action_index++] = tmp;
}

void retrace() {
  mode = RETRACE;
  action_index--;
  while (action_index >= 0){
    switch (actions[action_index].command) {
      case 'F':
        forward(actions[action_index]._speed);
        break;
      case 'B':
        backard(actions[action_index]._speed);
        break;
      case 'L':
        left(actions[action_index]._speed);
        break;
      case 'R':
        right(actions[action_index]._speed);
        break;
      case 'G':
        forwardLeft(actions[action_index]._speed);
        break;
      case 'I':
        forwardRight(actions[action_index]._speed);
        break;
      case 'H':
        backwardLeft(actions[action_index]._speed);
        break;
      case 'J':
        backwardRight(actions[action_index]._speed);
        break;
    }
    delay(actions[action_index--].duration);
  }
  action_index++;
  mode = CONTROLED;
}

void autoControl()
{
  if(distance <= threshlod_distance)
  {
    Stop();
    delay(100);
    backard(auto_speed);
    delay(300);
    Stop();
    delay(200);
    distance_right = lookRight();
    delay(200);
    distance_left = lookLeft();
    delay(200);
  
    if(distance_right >= distance_left)
    {
      right(auto_speed);
      delay(time_to_turn);
      Stop();
    }else
    {
      left(auto_speed);
      delay(time_to_turn);
      Stop();
    }
  }else
  {
    forward(auto_speed);
    delay(500);
  }
  distance = readPing();
}

int lookRight()
{
    myservo.write(SERVO_LEFT); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(SERVO_INIT); 
    return distance;
}

int lookLeft()
{
    myservo.write(SERVO_RIGHT); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(SERVO_INIT); 
    return distance;
}

int readPing() { 
  delay(70);
  int cm = sonar.ping() / US_ROUNDTRIP_CM;
  if(cm==0)
    cm = MAX_DISTANCE;
  return cm;
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

void runMotorWithSpeed(AF_DCMotor *motor, int motor_command, int motor_speed) {
  motor->setSpeed(motor_speed);
  motor->run(motor_command);
}
