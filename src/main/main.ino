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
unsigned long headingStartTime = 0;
const unsigned long HEADING_TIMEOUT = 10000; // 10 seconds
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
        Serial.print("Waiting over heading to destination."); 
        return HeadingToDestination;
      }
      break;

    case TurnAroundComplete:
      if (currentState == TurningAround) {
        Serial.print("Turn around complete. Heading to destination.");    
        return HeadingToDestination;
      }
      break;

    case Stop:
      if (currentState == HeadingToDestination) {
        Serial.print("Stopping. Returning to Waiting.");    
        return Waiting;
      }
      break;

    case Continue:
      if (currentState == Stuck || currentState == AvoidingObstacle) {
        Serial.print("Continuing. Heading to destination.");    
        return HeadingToDestination;
      }
      break;

    case AvoidObstacle:
      if (currentState == HeadingToDestination) {
        Serial.print("Avoiding obstacle.");    
        return AvoidingObstacle;
      }
      break;

    case WaitForAssistance:
      if (currentState == AvoidingObstacle) {
        Serial.print("Waiting for assistance.");    
        return Stuck;
      }
      break;
  }

  // No valid transition → remain in current state
  return currentState;
}

VehicleState handleButtonPress() {  
  if (currentState == Waiting) {
      return determineState(Start);
  } else if (currentState == Stuck) {
      return determineState(Continue);
  } else {
     return determineState(Stop);
  }             
}

VehicleState actionState(VehicleState state) {
  switch(state) {
    case Waiting:
      Serial.print("Starting to wait."); 
      stop();
      return state;
    case TurningAround:
      Serial.print("Turning around.");      
      turnAround();
       return actionState(determineState(TurnAroundComplete));
    case HeadingToDestination:
      Serial.print("Heading to destination.");
      headingStartTime = millis();      
      driveForward();
      return state;
    case AvoidingObstacle:    
      Serial.print("Avoiding obstacle.");      
      return actionState(determineState(WaitForAssistance));      
    case Stuck:
      stop();
      return state;
  }
}

VehicleState transitionToState(VehicleState newState) {
  Serial.print("Transitioning from ");
  Serial.print(currentState);
  Serial.print(" to NEW STATE: ");
  Serial.println(newState);  
  currentState = newState; // Update state before actions to ensure correct behavior in actionState
  return actionState(currentState);  
}

void loop() {  
  VehicleState newState = currentState;  
  if (digitalRead(BUTTON_PIN) == LOW) {   
    Serial.println("Handling button press."); 
    newState = handleButtonPress();    
  }  

  while (newState != currentState){  
    newState = transitionToState(newState);
  } 

  if (currentState == HeadingToDestination) {
    // Timeout check
    if (millis() - headingStartTime > HEADING_TIMEOUT) {
        Serial.println("Timeout reached. Returning to Waiting.");
        currentState = actionState(determineState(Stop));
    }
  }
  
  delay(200); // Small delay to avoid button bounce issues
}
