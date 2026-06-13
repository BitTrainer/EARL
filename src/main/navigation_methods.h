// navigation_methods.h
#include <AFMotor.h>

AF_DCMotor motorBackRight(1, MOTOR12_64KHZ);
AF_DCMotor motorBackLeft(4, MOTOR12_64KHZ);
AF_DCMotor motorFrontRight(2, MOTOR12_64KHZ);
AF_DCMotor motorFrontLeft(3, MOTOR12_64KHZ);
const int MAX_SPEED = 255;

void prepareVehicle() {
    motorBackRight.setSpeed(MAX_SPEED);
      motorBackLeft.setSpeed(MAX_SPEED);
      motorFrontRight.setSpeed(MAX_SPEED);
      motorFrontLeft.setSpeed(MAX_SPEED);      
}

void driveForward() {
  motorBackRight.run(FORWARD);
  motorBackLeft.run(FORWARD);
  motorFrontRight.run(FORWARD);
  motorFrontLeft.run(FORWARD);
}

void reverse() {
  motorBackRight.run(BACKWARD);
  motorBackLeft.run(BACKWARD);
  motorFrontRight.run(BACKWARD);
  motorFrontLeft.run(BACKWARD);
}

void stop() {
  motorBackRight.run(RELEASE);
  motorBackLeft.run(RELEASE);
  motorFrontRight.run(RELEASE);
  motorFrontLeft.run(RELEASE);
} 

void turnLeft() {
  stop();
  motorBackRight.run(FORWARD);
  motorFrontRight.run(FORWARD);
  delay(1000);
}

void turnRight() {
  stop();
  motorBackLeft.run(FORWARD);
  motorFrontLeft.run(FORWARD);
  delay(1000);
}

void turnAround() {    
    reverse();
    delay(700);   
    stop();
    turnLeft();
    stop();
    delay(700);   // EARL’s approximate 180° turn time
    stop();
    delay(100);   // Allow time to stop before next action
}

