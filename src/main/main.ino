// main.ino
#include "navigation_methods.h"

enum VehicleState {
  Waiting,
  TurningAround,
  HeadingToDestination,  
  AvoidingObstacle,
  Stuck,
};

enum VehicleEvent {  
  Start,
  TurnAroundComplete,
  Stop,
  Continue,
  AvoidObstacle,
  WaitForAssistance
 };

const int BUTTON_PIN = 9;  
VehicleState currentState = Waiting;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
  prepareVehicle();        
}

VehicleState determineState(VehicleEvent event) {

  switch(event) {

    case Start:
      if (currentState == Waiting) {
        return TurningAround;
      }
      break;

    case TurnAroundComplete:
      if (currentState == TurningAround) {
        return HeadingToDestination;
      }
      break;

    case Stop:
      if (currentState == HeadingToDestination) {
        return Waiting;
      }
      break;

    case Continue:
      if (currentState == Stuck || currentState == AvoidingObstacle) {
        return HeadingToDestination;
      }
      break;

    case AvoidObstacle:
      if (currentState == HeadingToDestination) {
        return AvoidingObstacle;
      }
      break;

    case WaitForAssistance:
      if (currentState == AvoidingObstacle) {
        return Stuck;
      }
      break;
  }

  // No valid transition → remain in current state
  return currentState;
}

void handleButtonPress() {  
  if (currentState == Waiting) {
      currentState = determineState(Start);
  } else if (currentState == Stuck) {
      currentState = determineState(Continue);
  } else {
      currentState = determineState(Stop);
  }             
}

void actionCurrentState() {
  switch(currentState) {
    case Waiting:
      stop();
      break;
    case TurningAround:
      turnAround();
      currentState = determineState(TurnAroundComplete);
      break;
    case HeadingToDestination:
      driveForward();
      break;    
    case AvoidingObstacle:    
      currentState = determineState(waitForAssistance);
      break;
    case Stuck:
      stop();
      break;
  }

}

void loop() {  
  if (digitalRead(BUTTON_PIN) == LOW) {    
    handleButtonPress();  
  }  

  actionCurrentState();

  delay(50); 
}
