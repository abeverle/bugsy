#include "HBridge.h"

HBridge::HBridge(byte en, byte in1, byte in2){
    this->pin_en = en;
    this->pin_in1 = in1;
    this->pin_in2 = in2;
    this->inverted = false;
}

void HBridge::invert(){
    this->inverted = !this->inverted;
}

void HBridge::init(){
    pinMode(pin_en, OUTPUT);
    analogWrite(pin_en, 0);
    pinMode(pin_in1, OUTPUT);
    pinMode(pin_in2, OUTPUT);
}

void HBridge::drive(byte spd, bool dir){
    if(spd == 0){
        brake();
        return;
    }
    if(dir != this->inverted){
      digitalWrite(pin_in1, HIGH);
      digitalWrite(pin_in2, LOW);
    }
    else{
      digitalWrite(pin_in1, LOW);
      digitalWrite(pin_in2, HIGH);
    }
    analogWrite(pin_en, spd);
}

void HBridge::brake(){
    analogWrite(pin_en, 255);
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, HIGH);
}
