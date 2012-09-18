#include <SoftwareSerial.h>

int motor1 = 0;
int motor2 = 0;

SoftwareSerial SWSerial(2, 3); // RX on pin 2 (unused), TX on pin 3 (to S1).

void setup(){
  SWSerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Sabertooth listener started. Waiting for first packet...");
}

void loop(){
  char packet[4];
  //Clear characters until we get an address
  while(SWSerial.peek()<128){
    if(SWSerial.peek() == -1)
      Serial.println("Listener timed out!");
    else
      SWSerial.read();
  }
  //Read a packet
  SWSerial.readBytes(packet,4);
  char address = packet[0];
  char command = packet[1];
  char data = packet[2];
  //Check checksum
  if(((address + command + data)&127) != packet[3]){
    Serial.println("Bad Packet: Checksum failed!");
  }
  //Check that command is in range
  else if(command > 7 || command <0){
    Serial.print("Bad Packet: Command Out of Range(");
    Serial.print(command);
    Serial.println(")!");
  }
  else if(data > 127 || data <0){
    Serial.print("Bad Packet: Data Out of Range(");
    Serial.print(data);
    Serial.println(")!");
  }
  else{
    Serial.print("Address:");
    Serial.print(address);
    switch(command){
    case 0:
      motor1 = data;
      break;
    case 1:
      motor1 = -data;
      break;
    case 4:
      motor2 = data;
      break;
    case 5:
      motor2 = -data;
      break;
    case 6:
      motor1 = 2 * (data - 64);
      break;
    case 7:
      motor2 = 2 * (data - 64);
      break;
    }
    Serial.print(" M1:");
    Serial.print(motor1);
    Serial.print(" M2:");
    Serial.println(motor2);
  }
}


