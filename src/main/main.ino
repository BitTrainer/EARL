#include "include/navigator.h"
const int buttonPin = 9;  
Navigator * navigator = nullptr;
bool isStopped = true ; 

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  navigator = new Navigator();
}

void loop() {  
  
  if (digitalRead(buttonPin) == LOW) {
    if (isStopped) {
      Serial.println("Driving forward...");      
      navigator->driveForward();
    } else {
      Serial.println("stopped");
      navigator->stop();
    }    
    
    delay(300);
    isStopped = !isStopped;
  }  
}
