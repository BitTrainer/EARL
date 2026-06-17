/**
 * Navigation methods for controlling the vehicle's movement.
 */

#include <AFMotor.h>

AF_DCMotor motorBackRight(1, MOTOR12_64KHZ);
AF_DCMotor motorBackLeft(4, MOTOR12_64KHZ);
AF_DCMotor motorFrontRight(2, MOTOR12_64KHZ);
AF_DCMotor motorFrontLeft(3, MOTOR12_64KHZ);
const int FAST_SPEED = 255;
const int SLOW_SPEED = 100;

void prepareVehicle() {
    motorBackRight.setSpeed(FAST_SPEED);
      motorBackLeft.setSpeed(FAST_SPEED);
      motorFrontRight.setSpeed(FAST_SPEED);
      motorFrontLeft.setSpeed(FAST_SPEED);      
}

void driveForward(int speed) {
  motorBackRight.setSpeed(speed);
  motorBackLeft.setSpeed(speed);
  motorFrontRight.setSpeed(speed);
  motorFrontLeft.setSpeed(speed);

  motorBackRight.run(FORWARD);
  motorBackLeft.run(FORWARD);
  motorFrontRight.run(FORWARD);
  motorFrontLeft.run(FORWARD);
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
