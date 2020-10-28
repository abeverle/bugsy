#include "drive.h"
#include "math.h"

Drive::Drive(HBridge *L, HBridge *R){
    this->left = L;
    this->right = R;
}

double Drive::speed2power(double spd){
    return (55.552 - sqrt(-84.752 * spd + 1928.56664))/42.376;
}

void Drive::go(double v, double w){
    double vr = v + (w * 5 / 2);
    double vl = v - (w * 5 / 2);

    double vm = max(abs(vl), abs(vr));

    Serial.println(vl);
    Serial.println(vr);

    vr = vr / vm * 20;
    vl = vl / vm * 20;

    bool dl = vl < 0;
    bool dr = vr < 0;

    vl = this->speed2power(abs(vl)) * 255;
    vr = this->speed2power(abs(vr)) * 255;
    left->drive((byte)vl, dl);
    right->drive((byte)vr, dr);
}

void Drive::stop(){
    this->left->brake();
    this->right->brake();
}