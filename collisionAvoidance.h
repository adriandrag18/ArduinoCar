#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  A1  
#define ECHO_PIN     A0  
#define MAX_DISTANCE 250 
#define SERVO_INIT 90
#define SERVO_LEFT 45
#define SERVO_RIGHT 135

extern Servo myservo;
extern NewPing sonar;

extern int readPing();
extern int lookLeft();
extern int lookRight();

#endif