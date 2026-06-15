/** 
 * main.ino
 * The main logic of EARL.
 *  
 **/ 

#include "navigation_methods.h"
#include "visual_indicator_methods.h"

enum VehicleState {
  Waiting,
  TurningAround,
  HeadingToDestination,  
  AvoidingObstacle,
  Stuck,
};

enum VehicleEvent {  
  buttonPress  ,
  drivingTimeout
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
  indicateWaiting();   
}

// See which state EARL should be in.
VehicleState determineState(VehicleEvent event) {
  switch(event) {
    case buttonPress:
      if (currentState == Waiting) {
        return HeadingToDestination;
      } else {
        return Waiting;
      }
    case drivingTimeout:
      if (currentState == HeadingToDestination) {
        return Waiting;
      }
    default:
      return currentState; // No state change for unhandled events
  }
}

// Any actions that need to be performed when transitioning between states can be handled here.
VehicleState performStateTransitionAction(VehicleState fromState, VehicleState toState) { 
  switch(toState) {
    case Waiting:       
      if (fromState != Waiting) {
         Serial.println("Stopping.");
         stop();
      }      
      indicateWaiting();
      return toState;
    case HeadingToDestination:
      if (fromState != HeadingToDestination) {
         Serial.println("Starting to drive towards destination.");
         headingStartTime = millis();
         driveForward();
      }       
      indicateHeadingToDestination();
      return toState;
    }
}

// Change state 
void transitionToState(VehicleState newState) {
  if (newState != currentState) {
    Serial.print("Transitioning from ");
    Serial.print(currentState);
    Serial.print(" to NEW STATE: ");
    Serial.println(newState);  
  }
  
  currentState = performStateTransitionAction(currentState, newState);  
}

bool hasDriveTimedOut() {
  return (millis() - headingStartTime) >= HEADING_TIMEOUT;
}

VehicleState handleButtonPress() {  
  Serial.println("Handling button press. ");
  return determineState(buttonPress);
}

// The main loop that runs until the unit is switched off.
void loop() {  
   VehicleState newState = currentState;  
   
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button press. ");
    newState = handleButtonPress();
  } else if (currentState == HeadingToDestination) {    
    if (hasDriveTimedOut()) {
        Serial.println("Timeout reached. Returning to Waiting.");
        newState = determineState(drivingTimeout);
    } 
  }

  transitionToState(newState);  
  delay(DEBOUNCE_DELAY); // Debounce to stop over-triggering on button press
}
