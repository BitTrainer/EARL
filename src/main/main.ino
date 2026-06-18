/** 
 * main.ino
 * The main logic of EARL.
 *  
 **/ 

#include "navigation_methods.h"
#include "visual_indicator_methods.h"
#include "ultrasonic.h"

// Settings
const long DISTANCE_TO_OBSTACLE_SLOW = 500; // When an object is 300mm or closer it will be considered an obstacle. 
const long DISTANCE_WHEN_OBJECT_IS_OBSTACLE = 350; // When an object is 300mm or closer it will be considered an obstacle. 
const int DEBOUNCE_DELAY = 450; // milliseconds
const unsigned long HEADING_TIMEOUT = 5000; // 5 seconds
const int BUTTON_PIN = 9;

// The main logic
enum VehicleState {
  Waiting,
  TurningAround,
  HeadingToDestinationFast, 
  HeadingToDestinationSlow,
  Stuck,
};

enum VehicleEvent {  
  buttonPress  ,
  drivingTimeout,
  distantObstacleDetected,
  obstructionDetected,
  clearPathDetected,
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
        return HeadingToDestinationFast;
      } else {
        return Waiting;
      }
    case drivingTimeout:
      if (currentState == HeadingToDestinationFast || currentState == HeadingToDestinationSlow) {
        return Waiting;
      }
    case obstructionDetected:
      if (currentState == HeadingToDestinationFast || currentState == HeadingToDestinationSlow) {
        return Stuck;
      }  
    case distantObstacleDetected:
      if (currentState == HeadingToDestinationFast) {
        return HeadingToDestinationSlow;
      } else {
        return currentState; // No state change if not currently heading to destination
      }  
    case clearPathDetected:
      if (currentState == HeadingToDestinationSlow) {
        return HeadingToDestinationFast;
      } 
      return currentState; // No state change if not currently heading to destination slow  
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
    case HeadingToDestinationFast:    
      if (fromState != HeadingToDestinationFast) {
         Serial.println("Starting to drive towards destination.");
         headingStartTime = millis();
         driveForward(FAST_SPEED);
      }             
      indicateHeadingToDestination();
      return toState;
     case HeadingToDestinationSlow:    
      if (fromState != HeadingToDestinationSlow) {
         Serial.println("Slowing down due to obstacle in distance.");         
         driveForward(SLOW_SPEED);
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

VehicleEvent obstacleDistanceState(long distanceToObject) {
  if (distanceToObject > 0 
       && distanceToObject <= DISTANCE_WHEN_OBJECT_IS_OBSTACLE) {
    return obstructionDetected;
  } else if (distanceToObject > DISTANCE_WHEN_OBJECT_IS_OBSTACLE
              && distanceToObject <= DISTANCE_TO_OBSTACLE_SLOW) {
    return distantObstacleDetected;
  } else {
    return clearPathDetected;
  }
}

VehicleState handleButtonPress() {     
  Serial.println("Handling button press. ");  
  return determineState(buttonPress); 
}

// The main loop that runs until the unit is switched off.
void loop() {  
  VehicleState newState = currentState;  

  long distanceToObjectAhead = frontSensor.distanceMM();
  newState = determineState(obstacleDistanceState(distanceToObjectAhead));
  transitionToState(newState);

  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button press. ");
    newState = handleButtonPress();   
  } else if (currentState == HeadingToDestinationFast || currentState == HeadingToDestinationSlow) {    
    if (hasDriveTimedOut()) {
        Serial.println("Timeout reached. Returning to Waiting.");
        newState = determineState(drivingTimeout);
    } 
  }

   transitionToState(newState);   
   delay(DEBOUNCE_DELAY); 
}
