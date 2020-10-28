#ifndef DRIVE_H
#define DRIVE_H

#include "HBridge.h"

class Drive{
public:
    Drive(HBridge *, HBridge *);

    // v in inches/sec
    // w in radians/sec
    void go(double v, double w);
    void stop();
private:
    double speed2power(double spd);
    HBridge *left, *right;
};

#endif