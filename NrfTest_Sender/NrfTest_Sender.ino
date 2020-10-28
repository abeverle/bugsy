#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

byte msg[4];// = {7,0,2,0};
RF24 radio(9, 10);

int ackData[2] = {-1,-1};
bool newData = false;

const uint64_t pipe = 0xE6E6E6E6E6E6;
//const byte slaveAddress[5] = {'R','x','A','A','A'};

void setup() {
  Serial.begin(9600);

  radio.begin();

  radio.enableAckPayload();
  radio.setRetries(5, 5);
  
  radio.openWritingPipe(pipe);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  while(Serial.available() < 3){
    delay(20);
  } 
  Serial.readBytes(msg, 3);

  bool result = radio.write(msg, sizeof(msg));
  if(result) {
    if(radio.isAckPayloadAvailable()) {
      radio.read(&ackData, sizeof(ackData));
      newData = true;
      Serial.print(ackData[0]);
      Serial.print(", ");
      Serial.println(ackData[1]);
    }
    else {
      Serial.println("Acknowledged but no data");
    }
  }
  else {
    Serial.println("failed to send... not ack");
  }
}
