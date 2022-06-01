#include <Servo.h>
#include <NewPing.h>
#include <AFMotor.h>


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
#define MAX_ACTIONS 100

Servo myservo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

typedef struct {
  char command;
  int speed;
  int activation_time;  
} action;

float speed_factor_diagonal_move = 0.5;

char command;
int controled_speed = MAX_SPEED / 2;
int auto_speed = 110;
int distance = 0;
int distance_left = 0;
int distance_right = 0;
int mode = CONTROLED;
int threshlod_distance = 25;
int time_to_turn = 850;

action actions[MAX_ACTIONS];
int action_index = 0;

void setup() {
  myservo.attach(9);  // servo2
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    
    decodeCommand();
  }
  if (mode == AUTO) {
    autoControl();
  }
}

void decodeCommand()
{
  switch (command) {
    case 'F':
      mode = CONTROLED;
      actions[action_index++] = {'B', controled_speed, 50};
      forward(controled_speed);
      break;
    case 'B':
      mode = CONTROLED;
      actions[action_index++] = {'F', controled_speed, 50};
      backard(controled_speed);
      break;
    case 'L':
      mode = CONTROLED;
      actions[action_index++] = {'R', controled_speed, 50};
      left(controled_speed);
      break;
    case 'R':
      mode = CONTROLED;
      actions[action_index++] = {'L', controled_speed, 50};
      right(controled_speed);
      break;
    case 'G':
      mode = CONTROLED;
      actions[action_index++] = {'I', controled_speed, 50};
      forwardLeft(controled_speed);
      break;
    case 'I':
      mode = CONTROLED;
      actions[action_index++] = {'G', controled_speed, 50};
      forwardRight(controled_speed);
      break;
    case 'H':
      mode = CONTROLED;
      actions[action_index++] = {'J', controled_speed, 50};
      backwardLeft(controled_speed);
      break;
    case 'J':
      mode = CONTROLED;
      actions[action_index++] = {'H', controled_speed, 50};
      backwardRight(controled_speed);
      break;
    case 'S':
      if (mode == CONTROLED)
        Stop();
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
