/**
 * Visual indicator methods for 
 * controling LEDs and indicators that 
 * show what EARL is doing.
 */

const int LED_PIN = 8;

void setupIndicators() {
  pinMode(LED_PIN, OUTPUT);
}

// Turn on the LED to indicate that EARL is waiting.
void indicateWaiting() {  
    digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate waiting    
}

// Flash the LED to indicate that EARL is heading towards the destination.
void indicateHeadingToDestination() {  
  for (int i = 0; i < 10; i++) {
     digitalWrite(LED_PIN, LOW); 
     delay(100); 
     digitalWrite(LED_PIN, HIGH); 
     delay(100); 
  } 
}