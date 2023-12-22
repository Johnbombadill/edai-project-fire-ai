#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include <ArduinoQueue.h>
#include "main.h"
#include "serial.h"
#include "encoder.h"
#include "motorcontrol.h"
#include "led.h"
#include "gyro.h"
#include "config.h"
#include "localization.h"
#include "current_state.h"


/*
 * Following are variables used as global variables which determine what the robot shoud do.
 * "currentState" and "currentDirection" is the primary source for the robot for what it should do when acting or moving.
 * These are defined by the typedef enum "robotState_t" and "direction_t" which have various parameters.
 * 
 * Example: currentState = STANDBY; - this makes sure that the robot changes to standby-mode in the global loop-function.
 * 
 * Example: currentDirection = NONE; - this makes sure that the robot does not move in any direction, even if it is told to do so with a ceratain speed.
 * 
 * 
 * Beneath are decalarations on where the sensors, the motors and the RBG-LEDs are connected in the Arduino.
 * How to use can be found in repective .ino file.
 */

//Initiliazes sensors, serial, ports etc.
void setup() {  
  setupSerial();
  setupEncoderInterrupts();
  setupMotors();
  setCurrentState(STANDBY);
  setupLED();
  setupGyro();
  randomSeed(analogRead(0));
}

/*
 * The following "loop()" is the main program of the Arduino.
 * 
 * It initializes by performing a serial tick, meaning that it will firstly check whether the Arduino should update based on timed
 * update-times but secondly read the data and store it for further use, seen in serial.ino
 * 
 * The loop is designed to be structured in various self-explanatory states such as: standby and manual.
 * The standby-mode is simply a state where the robot is stationary and simply awaits orders.
 * Manual is the state where you MANUALLY control the robot via serial communication.
 * 
 */
void loop() {
  doSerialTick();

  switch(getCurrentState()){
    case(STANDBY):
      resetStateLEDs();
      stopMotors();
      break;
    case(MANUAL):
      resetStateLEDs();
      doManualControlTick();
      break;
  }
}


