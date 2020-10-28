#ifndef H_BRIDGE_H
#define H_BRIDGE_H

#include <Arduino.h>

class HBridge{
private:
    byte pin_en, pin_in1, pin_in2;
    bool inverted;
public:
    HBridge(byte en, byte in1, byte in2);

    void drive(byte spd, bool dir);
    void brake();
    void init();
    void invert();
};

#endif
