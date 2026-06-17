/** 
 * main.ino
 * The main logic of EARL.
 *  
 **/ 

#include "navigation_methods.h"
#include "visual_indicator_methods.h"
#include "ultrasonic.h"

// Settings
const long DISTANCE_WHEN_OBJECT_IS_OBSTACLE = 300; // When an object is 300mm or closer it will be considered an obstacle. 
const int DEBOUNCE_DELAY = 450; // milliseconds
const unsigned long HEADING_TIMEOUT = 5000; // 5 seconds
const int BUTTON_PIN = 9;

// The main logic
enum VehicleState {
  Waiting,
  TurningAround,
  HeadingToDestination,    
  Stuck,
};

enum VehicleEvent {  
  buttonPress  ,
  drivingTimeout,
  obstacleDetected,
 };


void setup() {  
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setupIndicators();
  prepareVehicle();        
  indicateWaiting();   
}

// Global variables
unsigned long headingStartTime = 0;
VehicleState currentState = Waiting; // After being switched on, EARL starts in the Waiting state, waiting for the button press to start heading to the destination.
Ultrasonic frontSensor(7, 10); // TRIG=7, ECHO=10

// See which state EARL should be in.
VehicleState determineState(VehicleEvent event) {
  switch(event) {
    case buttonPress:
      if (currentState == Waiting || currentState == Stuck) {
        return HeadingToDestination;
      } else {
        return Waiting;
      }
    case drivingTimeout:
      if (currentState == HeadingToDestination) {
        return Waiting;
      }
    case obstacleDetected:
      if (currentState == HeadingToDestination) {
        return Stuck;
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
     case Stuck:
      if (fromState != Stuck) {
         Serial.println("Vehicle is stuck. Stopping.");
         stop();
      }       
      indicateWaiting(); // Reuse waiting indicator to indicate being stuck
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

bool isObstacleAhead(long distanceToObject) {
  return distanceToObject > 0 
       && distanceToObject <= DISTANCE_WHEN_OBJECT_IS_OBSTACLE;
}

VehicleState handleButtonPress() {     
  Serial.println("Handling button press. ");  
  return determineState(buttonPress); 
}

// The main loop that runs until the unit is switched off.
void loop() {  
   VehicleState newState = currentState;  
   long distanceToObjectAhead = frontSensor.distanceMM();

  if (isObstacleAhead(distanceToObjectAhead)) {
    Serial.println("Obstacle detected!");
    newState = determineState(obstacleDetected);
  } else if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button press. ");
    newState = handleButtonPress();   
  } else if (currentState == HeadingToDestination) {    
    if (hasDriveTimedOut()) {
        Serial.println("Timeout reached. Returning to Waiting.");
        newState = determineState(drivingTimeout);
    } 
  }

  transitionToState(newState);   
   delay(DEBOUNCE_DELAY); 
}
