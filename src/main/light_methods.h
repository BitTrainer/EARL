const int LED_PIN = 8;

void indicateWaiting() {  
    digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate waiting    
}

void indicateHeadingToDestination() {  
  for (int i = 0; i < 10; i++) {
     digitalWrite(LED_PIN, LOW); 
     delay(100); 
     digitalWrite(LED_PIN, HIGH); 
     delay(100); 
  } 
}