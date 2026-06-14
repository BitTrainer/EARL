// navigation_methods.h
#include <AFMotor.h>

AF_DCMotor motorBackRight(1, MOTOR12_64KHZ);
AF_DCMotor motorBackLeft(4, MOTOR12_64KHZ);
AF_DCMotor motorFrontRight(2, MOTOR12_64KHZ);
AF_DCMotor motorFrontLeft(3, MOTOR12_64KHZ);

const int MAX_SPEED = 255;
const int TURNING_SPEED = 200; // Reduced speed for turning to improve control

void setAllMotorSpeeds(int speed) {
  motorBackRight.setSpeed(speed);
  motorBackLeft.setSpeed(speed);
  motorFrontRight.setSpeed(speed);
  motorFrontLeft.setSpeed(speed);      
}

void prepareVehicle() {
  setAllMotorSpeeds(MAX_SPEED);      
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
  int turnDuration = 2450; // Adjust this value based on testing for a 180° turn
  int reverseDuration = 900; // Adjust this value based on testing for sufficient space to turn  

  reverse();
  delay(reverseDuration); // Move backward for a short duration to create space for the turn
  setAllMotorSpeeds(TURNING_SPEED); // Reduce speed for more controlled turn  
  turnLeft();  
  delay(turnDuration);   // EARL’s approximate 180° turn time
  stop();
  delay(50);   // Allow time to stop before next action    
  setAllMotorSpeeds(MAX_SPEED); // Restore max speed for driving forward
}

