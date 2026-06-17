/**
 * @file ultrasonic.h
 */

 class Ultrasonic {
  int trigPin;
  int echoPin;
public:
  Ultrasonic(int trig, int echo) : trigPin(trig), echoPin(echo) {}

  void begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  long distanceMM() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);
    return (duration * 0.343) / 2; // mm
  }
};
