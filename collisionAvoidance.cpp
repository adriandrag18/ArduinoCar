#include <Servo.h>
#include <NewPing.h>
#include "collisionAvoidance.h"

#define TRIGGER_PIN  A1  
#define ECHO_PIN     A0  
#define MAX_DISTANCE 250 

Servo myservo;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int readPing() { 
  delay(70);
  int cm = sonar.ping() / US_ROUNDTRIP_CM;
  if(cm==0)
    cm = MAX_DISTANCE;
  return cm;
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

