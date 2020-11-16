
#include "HBridge.h"
#include "SmartServo.h"
#include "Eyes.h"
#include "drive.h"
#include <math.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

#define EN1 10
#define EN2 5
#define IN1A 9
#define IN1B 8
#define IN2A 7
#define IN2B 6

HBridge back(EN1, IN1A, IN1B);
HBridge turn(EN2, IN2A, IN2B);

const int MESSAGE_SIZE = 5;
const int ACK_SIZE = 2;
byte msg[MESSAGE_SIZE];
byte lastMsg[MESSAGE_SIZE];
RF24 radio(2, 4);
const uint64_t pipe = 0xE6E6E6E6E6E6;

//holds wheel revolutions, distance to ultrasonic
int ackData[ACK_SIZE] = {0, 50};
bool newData = false;
unsigned long lastMsgTime;

int odoPin = A2;
int odoSignal = 0;
int revolutions = 0;
int totalRevolutions = 0;
bool addRev = true;

// 80 is straight forward
Servo sv;
Eyes myEyes;

void readFromRadio();
void move();
bool instructionAllowed(byte dir, byte ctrv);
void updateRev();
bool newMessage();
void displayMsg();
void initializeRadio();

void setup(){
    pinMode (odoPin, INPUT);

    initializeRadio();

    lastMsgTime = 0;
    turn.invert();
//    back.invert();
    pinMode(A5, OUTPUT);

    Serial.begin(9600);    
    Serial.println("Started robot");

    setHome();
}

void loop() {
    readFromRadio();
    radio.writeAckPayload(1, &ackData, sizeof(ackData));

    // More than 0.25 seconds since last message -> stop
    if(millis() - lastMsgTime > 250){
        msg[0] = 0;
        msg[1] = 0;
        msg[2] = 0;
    }
     
    displayMsg();
    updateRev();
    move();
}

void readFromRadio() {
    if(radio.available()){
        for(int i = 0; i < MESSAGE_SIZE; i++){
            lastMsg[i] = msg[i];
        }
        radio.read(msg, sizeof(msg));
        lastMsgTime = millis();
        newData = true;
    }    
}

void move() {
    byte ctrv = msg[0];
    byte ctrw = msg[1];
    byte dir = msg[2];
    byte maxRotations = msg[3];
    byte placeHolder = msg[4];

    if(newMessage())
        revolutions = 0;

    if(instructionAllowed(dir, ctrv)) {
        if(revolutions < maxRotations) { // || maxRotations = -1) { // if maxRotations is -1 its a user command not auto
        // if(revolutions >= 0) {
            back.drive(ctrv, dir & 1);
        }
        else{
            back.brake();
        }
        turn.drive(ctrw, dir & 2);
    }
    else {
        back.brake();
    }
}

bool instructionAllowed(byte dir, byte ctrv) {
    double distToObj = myEyes.checkForObject();
    ackData[1] = (int)distToObj;

    //overrides user input and doesnt allow them to run into things in front of them
    if(distToObj > 20)
        return true;
    else if((dir&1) == false)
        return true;
    else if(distToObj < 15){
        back.drive(ctrv, false); // Drives back a bit
        delay(100);
        return false;
    }
    else
        return false;
}

void updateRev() {
    odoSignal = analogRead(odoPin);
    
    if (odoSignal > 500 && addRev){
        revolutions++;
        totalRevolutions++;
        addRev = false;
    }
    else if(odoSignal < 200){
        addRev = true;
    }

    ackData[0] = revolutions;
    // Serial.print(revolutions*CIRCUMFERENCE);
    // Serial.print(", ");
    // Serial.println(revolutions);
}

bool newMessage() {
    bool newMsg = false;
    for(int i = 0; i < MESSAGE_SIZE; i++){
        if(lastMsg[i] != msg[i]){
            newMsg = true;
        }
    }
    return newMsg;
}

void setHome(){
    int homeRev = revolutions + 1;
    while (revolutions < homeRev) {
        back.drive(150, true); // drives forward
        updateRev();
    }
    back.brake();
    revolutions = 0;
    delay(100);
}

void displayMsg() {
//   if(newMessage()){
         for (int i = 0; i < 3; i++) {
             Serial.print(msg[i], HEX);
             Serial.print(" ");
         }
         Serial.print(msg[3]);
         Serial.print(", ");
//         Serial.print(msg[4]);
//         Serial.println("\n");
//   }
//     Serial.print(odoSignal);
//     Serial.print(" ");
     Serial.print(ackData[0]);
     Serial.print(" ");
     Serial.println(odoSignal);    
}

void initializeRadio() {
    radio.begin();
    radio.openReadingPipe(1, pipe);
    radio.enableAckPayload();
    radio.startListening();
    radio.writeAckPayload(1, &ackData, sizeof(ackData));
}
