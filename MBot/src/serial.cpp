#include "serial.h"
#include "localization.h"
#include "encoder.h"
#include "motorcontrol.h"
#include "current_state.h"
#include "led.h"
#include "temperature.h"

// Global variables used to store the latest messages received, time when the robot last got updated, and how many missed messages we have missed
String recievedMessage;
long timeAtLastSerialUpdate;
int numberOfTicksMissed = 0;
int numberOfTicksSinceTemperatureTransmission = 0;

//Initiate serial communication
void setupSerial(){
  Serial.begin(57600); //Changed from 115200 due to unstable connection between Arduino and Pi
}

/*
 * This is the main function of the serial comunication.
 * When called, it updates what the serial monitor recieves periodically.
 * The update frequency can be changed in config.h.
 * 
 * If alot of messages are missed, then change to stanby state.
 * This ensures that the robot does not move when it does not recieve any commands.
 * The amount of max missed messages can be fine tuned in config.h is the robot does not move smooth when manually controlling it.
 * 
 * The functions used build on each other, and goes deep into other functions.
 * Therefore, it may be confusing to read.
 * It can definatly be developed in a better way, but it works for now according to our designed protocol.
 */
void doSerialTick(){
  if(millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastSerialUpdate = millis();
    
    readSerialData();
    
    clearStoredMessages();
  }

  if(numberOfTicksMissed >= MAX_ALLOWED_MISSED_SERIAL_TICKS){
    setCurrentDirection(NONE);
  }

  numberOfTicksSinceTemperatureTransmission += 1;
  if(numberOfTicksSinceTemperatureTransmission >= NUMBER_OF_TICKS_BETWEEN_TEMPERATURE_TRANSMISSION){
    numberOfTicksSinceTemperatureTransmission = 0;
    sendTemperatureTransmission(getCurrentTemperature());
  }
}

void sendTemperatureTransmission(int currentTemperature){
  String temperatureMessage = "t:" + String(currentTemperature);
  temperatureMessage.trim();
  Serial.println(temperatureMessage);
}

////As long as there are data available on the serial bus, read and store it, then acknowelge it to the Pi
void readSerialData(){
  if(Serial.available() > 0){
    numberOfTicksMissed = 0;
    
    recievedMessage = readSerialBus();

    setSerialDataRecieved(recievedMessage);

    ackMessage(getSerialDataRecieved());

    Serial.flush();
    
  }
  else{
    numberOfTicksMissed++;
  }
}

//Simply adds "!" to a message if sucessful, "?" if not
void ackMessage(String message){
  if(!ackReviecedMessage()){
    sendMessageNOK(message);
  }
}

//As long as there are data to read, read it. Also, making sure that newline character is found
String readSerialBus(){
  String message;
  bool EoLFound = false;
  while (Serial.available() > 0) {
    message += (char)Serial.read();
    if(message.indexOf("\n") > 0){
      EoLFound = true;
    }
  }
  if(EoLFound){
    return message;
  }
  else{
    return "EoL Not found";
  }
}

void setSerialDataRecieved(String s_dataToStore){
  recievedMessage = s_dataToStore;
}

String getSerialDataRecieved(){
  return recievedMessage;
}

//Some specific "?", "C" and coorinate functions used often
void sendMessageNOK(String message){
  message.trim();
  Serial.println(message + "?");
}

void sendMessageAck(String message){
  message.trim();
  Serial.println(message + "!");
}

void clearStoredMessages(){
  recievedMessage = "";
}

/*
 * The following code is used for decoding and using the messages recieved.
 * This is done with the help of switch cases and some enums.
 * Depening on what message is recieved, the robot should act accordingly.
 */
bool ackReviecedMessage(){
  switch(convertMessageToInt(getSerialDataRecieved())){
    case(Hello):
      sendMessageAck(getSerialDataRecieved());
      return true;

    case(Standby):
      setCurrentState(STANDBY);
      sendMessageAck(getSerialDataRecieved());
      //Reset map coordinates
      resetCoordinates();
      resetEncoderValues();
      return true;

    case(ManualStop):
      setCurrentState(MANUAL);
      setCurrentDirection(NONE);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualForward):
      setCurrentDirection(FORWARD);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualBackward):
      setCurrentDirection(BACKWARD);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualLeft):
      setCurrentDirection(LEFT);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualRight):
      setCurrentDirection(RIGHT);
      sendMessageAck(getSerialDataRecieved());
      return true;

    case(SetManualMotorSpeedHigh):
      setMotorSpeedManualPercentage(MANUAL_MOTOR_SPEED_HIGH_PERCENTAGE);
      sendMessageAck(getSerialDataRecieved());
      activateAllLEDsRGB(100, 100, 100);
      return true;
    case(SetManualMotorSpeedMedium):
      setMotorSpeedManualPercentage(MANUAL_MOTOR_SPEED_MEDIUM_PERCENTAGE);
      sendMessageAck(getSerialDataRecieved());
      activateAllLEDsRGB(100, 100, 100);
      return true;
    case(SetManualMotorSpeedLow):
      setMotorSpeedManualPercentage(MANUAL_MOTOR_SPEED_LOW_PERCENTAGE);
      sendMessageAck(getSerialDataRecieved());
      activateAllLEDsRGB(100, 100, 100);
      return true;

    case(Error):
      return false;
  }
  return false;
}

messageRecieved_t convertMessageToInt(String message){
  message.trim();
  
  if(message.equals("hello")){
    return Hello;
  }

  else if(message.equals("r")){ // R - Remain / Standby due to 'S' for backward
    return Standby;
  }
  else if(message.equals("c")){
    return ManualStop;
  }

  else if(message.equals("w")){
    return ManualForward;
  }
  else if(message.equals("d")){
    return ManualRight;
  }
  else if(message.equals("s")){
    return ManualBackward;
  }
  else if(message.equals("a")){
    return ManualLeft;
  }

  else if(message.equals("h")){
    return SetManualMotorSpeedHigh;
  }
  else if(message.equals("m")){
    return SetManualMotorSpeedMedium;
  }
  else if(message.equals("l")){
    return SetManualMotorSpeedLow;
  }

  else
    return Error;
}
