#include <Arduino.h>
#include "AFMotor.h"
#include "Servo.h"
#include "HCSR04.h"

/*==============================================================================================================*/

#define MOTORSPEED_RIGHT 255
#define MOTORSPEED_LEFT 255

#define PIN_SERVO 10
#define PIN_TRIGGER A0
#define PIN_ECHO A1

#define SERVO_MITTE 90
#define SERVO_RECHTS 40
#define SERVO_LINKS 140

#define WAIT_TURN 750
#define WAIT_SERVO 200

#define DIRECTION_FORWARD 1
#define DIRECTION_BACK 0

/*==============================================================================================================*/

void DriverForward();
void DriverBack();

/*==============================================================================================================*/

AF_DCMotor motor_rechts(4);
AF_DCMotor motor_links(2);
Servo servo;

bool direction = DIRECTION_FORWARD;

/*==============================================================================================================*/

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  // turn on motor
  motor_rechts.setSpeed(MOTORSPEED);
  motor_links.setSpeed(MOTORSPEED);
 
  motor_rechts.run(RELEASE);
  motor_links.run(RELEASE);
  

  servo.attach(PIN_SERVO);
  servo.write(SERVO_MITTE);

  pinMode(PIN_TRIGGER, INPUT);
  pinMode(PIN_ECHO, INPUT);

  HCSR04.begin(PIN_TRIGGER, PIN_ECHO);
}

/*==============================================================================================================*/

void loop() {
  double* distance;

  distance = HCSR04.measureDistanceCm();
  Serial.println("distance: ");
  Serial.println(*distance);

  if(direction == DIRECTION_FORWARD && *distance > 30) {
   
    DriverForward();
  }
  else {

    motor_rechts.run(RELEASE);
    motor_links.run(RELEASE);

    servo.write(SERVO_LINKS);
    delay(WAIT_SERVO);
    distance = HCSR04.measureDistanceCm();

    if(*distance > 30) {
      motor_rechts.run(BACKWARD);
      motor_links.run(FORWARD);

      direction = DIRECTION_FORWARD;
    }
    else {
      servo.write(SERVO_RECHTS);
      delay(WAIT_SERVO);
      distance = HCSR04.measureDistanceCm();

      if(*distance > 30) {
        motor_rechts.run(FORWARD);
        motor_links.run(BACKWARD);
        direction = DIRECTION_FORWARD;
      }
      else {
        DriverBack();
        direction = DIRECTION_BACK;

      }
    }
    
    servo.write(SERVO_MITTE);
    delay(WAIT_TURN);
  }
  

  delay(100);
  
}

/*==============================================================================================================*/

void DriverForward() {
  motor_rechts.run(FORWARD);
  motor_links.run(FORWARD);
}

/*==============================================================================================================*/

void DriverBack() {
  motor_rechts.run(BACKWARD);
  motor_links.run(BACKWARD);
}

/*==============================================================================================================*/
