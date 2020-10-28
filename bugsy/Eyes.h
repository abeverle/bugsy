#pragma once
#include <Arduino.h>

class Eyes {
    public:
        double checkForObject();
        double lastKnownDist;
    private:
        const int pingPin = A5; // Trigger Pin of Ultrasonic Sensor
        const int echoPin = A4; // Echo Pin of Ultrasonic Sensor  
        double microsecondsToInches(double microseconds);
        double microsecondsToCentimeters(double microseconds);
};
