#include <Servo.h>
#include <Arduino.h>

class SmartServo{
public:
    SmartServo(Servo *);
    void set(byte angle);
    void setOffset(byte angle);
    void attach(byte pin);
    int get();
private:
    Servo *sv;
    byte offset;
};