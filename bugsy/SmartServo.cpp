#include "SmartServo.h"

SmartServo::SmartServo(Servo *sv){
    this->sv = sv;
    this->offset = 0;
}

void SmartServo::attach(byte pin){
    if (!this->sv->attached()){
        this->sv->attach(pin);
    }
}

/* by default servo angle/direction is 0/left 90/front & 180/right
 *  The current configuration is 173/left 90/front & 3/right
*/
void SmartServo::set(byte angle){
    this->sv->write(angle + this->offset);
}

void SmartServo::setOffset(byte angle){
    this->offset = angle;
}

int SmartServo::get(){
    return this->sv->read() - this->offset;
}
