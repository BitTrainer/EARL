#include "navigation_methods.h"

enum VehicleState {
  Stopped,
  HeadingToDestination,
  AvoidingObstacle,
  Arrived
};

enum VehicleAction{  
  Start,
  Stop,
  AvoidObstacle
 };

const int BUTTON_PIN = 9;  
VehicleState currentState = Stopped;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  prepareVehicle();        
}

VehicleState performAction(VehicleAction action) {
  switch(action) {    
    case Start:
      Serial.println("Starting vehicle...");
      driveForward();
      return HeadingToDestination;
    case Stop:
      Serial.println("Stopping vehicle...");
      stop();
      return Stopped;
    case AvoidObstacle:
      Serial.println("Avoiding obstacle...");
      // Implementation for avoiding obstacle
      return AvoidingObstacle;
  }
}

void loop() {  

  if (digitalRead(BUTTON_PIN) == LOW) {
    if (currentState == Stopped) {
      currentState = performAction(Start);
    } else {
      currentState = performAction(Stop);
    }               
  }  

  delay(300); 
}
