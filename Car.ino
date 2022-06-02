#include <Servo.h>
#include <NewPing.h>
#include "motorControl.h"
#include "actionStack.h"

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

Servo myservo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

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



void setup() {
  myservo.attach(9);  // servo2
  Serial.begin(9600);
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

void retrace() {
  mode = RETRACE;
  while (!emtpyActionsStack()){
    action last_action = popAction();
    switch (last_action.command) {
      case 'F':
        forward(last_action._speed);
        break;
      case 'B':
        backard(last_action._speed);
        break;
      case 'L':
        left(last_action._speed);
        break;
      case 'R':
        right(last_action._speed);
        break;
      case 'G':
        forwardLeft(last_action._speed);
        break;
      case 'I':
        forwardRight(last_action._speed);
        break;
      case 'H':
        backwardLeft(last_action._speed);
        break;
      case 'J':
        backwardRight(last_action._speed);
        break;
    }
    delay(last_action.duration);
  }
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
