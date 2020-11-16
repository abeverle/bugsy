#include "Eyes.h"

//returns distance from object in front of robot in inches
double Eyes::checkForObject() {
    double duration, inches, cm;
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
    lastKnownDist = inches;
    return inches;
}

double Eyes::microsecondsToInches(double microseconds) {
   return microseconds / 74 / 2;
}

double Eyes::microsecondsToCentimeters(double microseconds) {
   return microseconds / 29 / 2;
}
