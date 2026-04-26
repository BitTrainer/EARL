#include <AFMotor.h>

AF_DCMotor motorBackRight(1, MOTOR12_64KHZ);
AF_DCMotor motorBackLeft(4, MOTOR12_64KHZ);
AF_DCMotor motorFrontRight(2, MOTOR12_64KHZ);
AF_DCMotor motorFrontLeft(3, MOTOR12_64KHZ);

void setup() {
  Serial.begin(9600);
  
  motorBackRight.setSpeed(255);
  motorBackLeft.setSpeed(255);
  motorFrontRight.setSpeed(255);
  motorFrontLeft.setSpeed(255);
}
void driveForward() {
  motorBackRight.run(FORWARD);
  motorBackLeft.run(FORWARD);
  motorFrontRight.run(FORWARD);
  motorFrontLeft.run(FORWARD);
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

void stop() {
  motorBackRight.run(RELEASE);
  motorBackLeft.run(RELEASE);
  motorFrontRight.run(RELEASE);
  motorFrontLeft.run(RELEASE);
}

void loop() {
  driveForward();
  delay(3000);
  stop();
  delay(3000);
}
