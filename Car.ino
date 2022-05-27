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

Servo myservo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command;
int controled_speed = MAX_SPEED / 2;
int auto_speed = 110;
int distance = 0;
int distance_left = 0;
int distance_right = 0;
int mode = CONTROLED;
int threshlod_distance = 25;
int time_to_turn = 850;

void setup() {
  myservo.attach(9);  // servo2
  Serial.begin(9600);
//  mode = AUTO;
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    
    decodeComand();
  }
  if (mode == AUTO) {
    autoControl();
  }
}

void decodeComand()
{
  switch (command) {
    case 'F':
      mode = CONTROLED;
      forward(controled_speed);
      break;
    case 'B':
      mode = CONTROLED;
      back(controled_speed);
      break;
    case 'L':
      mode = CONTROLED;
      left(controled_speed);
      break;
    case 'R':
      mode = CONTROLED;
      right(controled_speed);
      break;
    case 'S':
      if (mode == CONTROLED)
        Stop();
      break;
    case 'U':
      mode = AUTO;
      myservo.write(0);
      delay(500);
      myservo.write(SERVO_INIT);
      delay(500);
      break;
    case 'u':
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
    back(auto_speed);
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
  motor1.setSpeed(my_speed); //Define maximum velocity
  motor1.run(FORWARD);  //rotate the motor clockwise
  motor2.setSpeed(my_speed); //Define maximum velocity
  motor2.run(FORWARD);  //rotate the motor clockwise
  motor3.setSpeed(my_speed); //Define maximum velocity
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.setSpeed(my_speed); //Define maximum velocity
  motor4.run(FORWARD);  //rotate the motor clockwise
}

void back(int my_speed)
{
  motor1.setSpeed(my_speed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(my_speed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(my_speed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(my_speed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void left(int my_speed)
{
  motor1.setSpeed(my_speed); //Define maximum velocity
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.setSpeed(my_speed); //Define maximum velocity
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.setSpeed(my_speed); //Define maximum velocity
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.setSpeed(my_speed); //Define maximum velocity
  motor4.run(FORWARD);  //rotate the motor clockwise
}

void right(int my_speed)
{
  motor1.setSpeed(my_speed); //Define maximum velocity
  motor1.run(FORWARD);  //rotate the motor clockwise
  motor2.setSpeed(my_speed); //Define maximum velocity
  motor2.run(FORWARD);  //rotate the motor clockwise
  motor3.setSpeed(my_speed); //Define maximum velocity
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.setSpeed(my_speed); //Define maximum velocity
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void Stop()
{
  motor1.setSpeed(0);  //Define minimum velocity
  motor1.run(RELEASE); //stop the motor when release the button
  motor2.setSpeed(0);  //Define minimum velocity
  motor2.run(RELEASE); //rotate the motor clockwise
  motor3.setSpeed(0);  //Define minimum velocity
  motor3.run(RELEASE); //stop the motor when release the button
  motor4.setSpeed(0);  //Define minimum velocity
  motor4.run(RELEASE); //stop the motor when release the button
}
