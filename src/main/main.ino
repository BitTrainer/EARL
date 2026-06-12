#include "navigation_methods.h"

enum VehicleState {
  Stopped,
  Driving,
  AvoidingObstacle,
  Stuck
};

const int BUTTON_PIN = 9;  
VehicleState currentState = Stopped;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  prepareVehicle();  
}

void loop() {  

  if (digitalRead(BUTTON_PIN) == LOW) {
    if (currentState == Stopped) {
      Serial.println("Driving forward...");      
      driveForward();
      currentState = Driving;
    } else {
      Serial.println("stopped");
      stop();
      currentState = Stopped;
    }    
    
    delay(300);    
  }  
}
