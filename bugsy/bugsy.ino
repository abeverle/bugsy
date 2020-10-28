
#include "HBridge.h"
#include "SmartServo.h"
#include "Eyes.h"
#include "drive.h"
#include <math.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

byte msg[4];
byte lastMsg[4];
RF24 radio(2, 4);
const uint64_t pipe = 0xE6E6E6E6E6E6;
int ackData[2] = {87, -65};
bool newData = false;
unsigned long lastMsgTime;

int odoPin = A2;
int odoSignal = 0;
int revolutions = 0;
bool addRev = true;
const double CIRCUMFERENCE = 10.625;

// 80 is straight forward
Servo sv;

#define EN1 10
#define EN2 5
#define IN1A 9
#define IN1B 8
#define IN2A 7
#define IN2B 6

HBridge back(EN1, IN1A, IN1B);
HBridge turn(EN2, IN2A, IN2B);

Eyes myEyes;

void avoid();
void updateRev();

void setup(){
    pinMode (odoPin, INPUT);
    
    radio.begin();
    radio.openReadingPipe(1, pipe);
    radio.enableAckPayload();
    radio.startListening();
    radio.writeAckPayload(1, &ackData, sizeof(ackData));
//
//   sv.attach(3);
//   sv.write(80);
    lastMsgTime = 0;

    Serial.begin(9600);    
    Serial.println("Started robot");
    
}

void loop(){
    if(radio.available()){
        for(int i = 0; i < 4; i++){
            lastMsg[i] = msg[i];
        }
        radio.read(msg, sizeof(msg));
        lastMsgTime = millis();
        newData = true;
    }

    radio.writeAckPayload(1, &ackData, sizeof(ackData));

    // More than 0.25 seconds since last message -> stop
    if(millis() - lastMsgTime > 250){
        msg[0] = 0;
        msg[1] = 0;
        msg[2] = 0;
    }
    
    bool newMsg = false;
    for(int i = 0; i < 4; i++){
        if(lastMsg[i] != msg[i]){
            newMsg = true;
        }
    }

    if(newMsg){
        Serial.print(msg[0], HEX);
        Serial.print(" ");
        Serial.print(msg[1], HEX);
        Serial.print(" ");
        Serial.print(msg[2], HEX);
        Serial.println("\n");
    }

    byte ctrv = msg[0];
    byte ctrw = msg[1];
    byte dir = msg[2];

    double distToObj = myEyes.checkForObject();

    //overrides user input and doesnt allow them to run into things in front of them
    if(distToObj > 20)
        back.drive(ctrv, dir & 1);
    else if((dir&1) == false)
        back.drive(ctrv, dir & 1);
    else if(distToObj < 15){
        back.drive(ctrv, false); // Drives back a bit
        delay(100);
        back.brake();
        //after stopping call avoid
    }
    else
        back.brake();
        //after stopping call avoid
    
    updateRev();

    Servo * sv;
    SmartServo myEyeRotation(sv);
//    myEyeRotation.set(170);
//    delay(1000);
//    myEyeRotation.set(10);

    turn.drive(ctrw, dir & 2);
}
void updateRev() {
    odoSignal = analogRead(odoPin);
    
    if (odoSignal > 400 && addRev){
        revolutions++;
        addRev = false;
    }
    else if(odoSignal < 200){
        addRev = true;
    }

     ackData[0] = revolutions*CIRCUMFERENCE;
     ackData[1] = revolutions;
    // Serial.print(revolutions*CIRCUMFERENCE);
    // Serial.print(", ");
    // Serial.println(revolutions);

}

//write it here, move to smart drive, move to computer
void avoid() {
//    frontDist = myEyes.checkForObject();
//
//    leftDist = myEyes.checkForObject();
//    rightDist = myEyes.checkForObject();
    //choose to go left or right (assuming front is actually blocked)
    //choose right (testing)
    //reverse a lil
    //max right turn... until forward is >15
    //turn eyes opposite of way you turned... this case left
    //go straight until thing on left is gone/far away 
    //then turn left... eyes front and return control to user,
}
