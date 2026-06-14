// main.ino
#include "navigation_methods.h"
#include "light_methods.h"

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
const int DEBOUNCE_DELAY = 300; // milliseconds
unsigned long headingStartTime = 0;
const unsigned long HEADING_TIMEOUT = 5000; // 5 seconds
VehicleState currentState = Waiting; // After being switched on, EARL starts in the Waiting state, waiting for the button press to start heading to the destination.

void setup() {  
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setupIndicators();
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

bool hasDriveTimedOut() {
  return (millis() - headingStartTime) >= HEADING_TIMEOUT;
}

void loop() {  
   VehicleState newState = currentState;  
   
   if (digitalRead(BUTTON_PIN) == LOW) {   
    Serial.print("Handling button press.");
    Serial.print("Current state: ");
    Serial.println(currentState); 
    newState = handleButtonPress(); 
    Serial.print("Handled button press.");
    Serial.print("New state: ");
    Serial.println(newState);       
  }  

  while (newState != currentState){  
    newState = transitionToState(newState);
  } 

  if (currentState == HeadingToDestination) {    
    if (hasDriveTimedOut()) {
        Serial.println("Timeout reached. Returning to Waiting.");
        currentState = actionState(determineState(Stop));
    } else {
      Serial.println("Heading to destination.");
      indicateHeadingToDestination(); // visual feedback that EARL is driving towards the destination.
    }
  }

  if (currentState == Waiting) {    
     indicateWaiting(); // Visual feedback that EARL is waiting for the button press to start heading to the destination. 
  }

  delay(DEBOUNCE_DELAY); // Debounce delay to prevent multiple triggers from a single press
}
