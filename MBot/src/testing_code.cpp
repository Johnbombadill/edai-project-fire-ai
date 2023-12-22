/*
 * This file contains testing code used when developing the robot.
 * Most of the code has been deleted over time but here are some of the last examples used to test some functionalities.
 * Do not mistake this file for final testing of the robot
 */
#include "testing_code.h"
#include "config.h"
#include "motorcontrol.h"
#include "localization.h"
#include "serial.h"

bool TESTfirstTapeFound = false;
bool TESTsecondTapeFound = false;

void doDrivingInASquareTest(){
  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
      
  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);
  

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);



  rotateByDegrees(180, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  stopMotorsMS(3000);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  stopMotorsMS(3000);
  
  
  while(true){
    stopMotors();
  }
}

void doRotationTest(){
  rotateByDegrees(90, LEFT, MAX_MOTOR_SPEED);

  stopMotorsMS(3000);

  rotateByDegrees(180, RIGHT, MAX_MOTOR_SPEED);

  stopMotorsMS(3000);

  rotateByDegrees(270, RIGHT, MAX_MOTOR_SPEED);

  stopMotorsMS(3000);

  rotateByDegrees(360, LEFT, MAX_MOTOR_SPEED);
}
