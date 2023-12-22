#include "motorcontrol.h"
#include "config.h"
#include "led.h"
#include "encoder.h"
#include "gyro.h"
#include "localization.h"

direction_t currentDirection = NONE;
int motorSpeedManualPercentage = 100;

//Sets up PWM-control for motors
void setupMotors(){
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

void doManualControlTick(){
  /*
   * If joystick is wanted:
   * moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentJoysticSpeedLeftMotor, currentAngleJoystick),calculateRightMotorSpeed(currentJoysticSpeedRightMotor, currentAngleJoystick));
   */
   move(getCurrentDirection(), getMotorSpeedManualPercentage() * PERCENTAGE_TO_PWM_FACTOR);
}

int getMotorSpeedManualPercentage(){
  return motorSpeedManualPercentage;
}

void setMotorSpeedManualPercentage(int newMotorSpeedPercentage){
  if(newMotorSpeedPercentage >= 0 || newMotorSpeedPercentage <= 100){
    motorSpeedManualPercentage = newMotorSpeedPercentage;
  }
}

/*
 * This is the main mowing function
 * This takes one input and one speed and the robot moves accordingly.
 */
void move(direction_t direction, float speedVal)
{
  setCurrentDirection(direction);
  float leftSpeed = 0;
  float rightSpeed = 0;
  
  if(getCurrentDirection() == FORWARD){
    activateManualForwardLEDs();
    leftSpeed = -speedVal * MOTOR_DEVIATION_FACTOR;
    rightSpeed = speedVal;
  }
  else if(getCurrentDirection() == BACKWARD){
    activateManualBackwardLEDs();
    leftSpeed = speedVal * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -speedVal;
  }
  else if(getCurrentDirection() == LEFT){
    activateManualLeftLEDs();
    leftSpeed = -speedVal * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -speedVal;
  }
  else if(getCurrentDirection() == RIGHT){
    activateManualRightLEDs();
    leftSpeed = speedVal * MOTOR_DEVIATION_FACTOR;
    rightSpeed = speedVal;
  }
  else if(getCurrentDirection() == NONE) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  
  setEncoderPwm(1, leftSpeed);
  setEncoderPwm(2, rightSpeed);

  _loop();
}

//If we want the robot to move based on distance
void driveDistance(int millimeters, direction_t movingDirection, int motorSpeed){
  resetEncoderValues();

  while((abs(getDistanceTravelled()) < millimeters - MILLIMETER_DISTANCE_WHEN_FREE_ROLLING_AFTER_FULL_SPEED)){
    Serial.println(getDistanceTravelled());
    move(movingDirection, motorSpeed);
  }
  //Serial.println("Gyro in driveDistance: " + String(getGyroZ()));
}

//If we want the robot to move based on time
void driveTime(int ms, direction_t movingDirection, int motorSpeed){
  resetEncoderValues();
  long timeWhenDone = millis() + ms;

  while(millis() < timeWhenDone){
    move(movingDirection, motorSpeed);
  }
  //Serial.println("Gyro in driveTime: " + String(getGyroZ()));
}

//This function makes the robot rotate, this works with angles ove 360 degrees (making it do full turns as well)
void rotateByDegrees(int degreesToRotate, direction_t rotateLeftOrRight, int motorSpeed) {
  if(rotateLeftOrRight == LEFT){
    if(calculateFullCirclesNeeded(degreesToRotate) > 0){
      rotateFullCircles(calculateFullCirclesNeeded(degreesToRotate), rotateLeftOrRight, motorSpeed);
    }
    
    int calcDegreesToRotate = getGyroZ() - ((degreesToRotate - ROTATING_LEFT_MOMENTUM_OFFSET) % 360);

    if(calcDegreesToRotate >= -180){
      while(getGyroZ() > calcDegreesToRotate){
        move(LEFT, motorSpeed);
      }
    }
    else{
      int degreesDelta = abs(calcDegreesToRotate) % 180;
      int degreeToRotateTo = (180 - degreesDelta) * 1;
      
      bool wasInLowerValueHalf = false;
      bool wentOverHighestPeak = false;
      
      while(!wentOverHighestPeak){
        move(LEFT, motorSpeed);
        if(getGyroZ() >= -180 && getGyroZ() < 90){wasInLowerValueHalf = true;}
        if(wasInLowerValueHalf && getGyroZ() > 90){wentOverHighestPeak = true;}
      }
      while(getGyroZ() > degreeToRotateTo){
        move(LEFT, motorSpeed);
      }
    }
  }
  
  else if(rotateLeftOrRight == RIGHT){
    if(calculateFullCirclesNeeded(degreesToRotate) > 0){
      rotateFullCircles(calculateFullCirclesNeeded(degreesToRotate), rotateLeftOrRight, motorSpeed);
    }

    int calcDegreesToRotate = getGyroZ() + ((degreesToRotate - ROTATING_RIGHT_MOMENTUM_OFFSET) % 360);

    if(calcDegreesToRotate <= 180){
      while(getGyroZ() < calcDegreesToRotate){
        move(RIGHT, motorSpeed);
      }
    }
    else{
      int degreesDelta = abs(calcDegreesToRotate) % 180;
      int degreeToRotateTo = (180 - degreesDelta) * -1;
      
      bool wasInUpperValueHalf = false;
      bool wentOverHighestPeak = false;
      
      while(!wentOverHighestPeak){
        move(RIGHT, motorSpeed);
        if(getGyroZ() > 0 && getGyroZ() <= 180){wasInUpperValueHalf = true;}
        if(wasInUpperValueHalf && getGyroZ() < -90){wentOverHighestPeak = true;}
      }
      while(getGyroZ() < degreeToRotateTo){
        move(RIGHT, motorSpeed);
      }
    }
  }
  else{
    //Serial.println("Recieved wrong parameter in: rotateByDegrees");
  }
  stopMotorsMS(200);
}

//Takes the amount of cirles you want to make the robot turn and does so with the initial gyro value as start and end point
void rotateFullCircles(int amountOfCircles, direction_t rotateLeftOrRight, int motorSpeed){
  for(int i = 0; i < amountOfCircles; i++){
    int angleNow = getGyroZ();
    
    bool beenInFirstQuadrant = false;
    bool beenInSecondQuadrant = false;
    bool beenInThirdQuadrant = false;
    bool beenInFourthQuadrant = false;
    
    bool circleRotated = false;

    //Rotate until first half of the circle is completed
    while(!circleRotated){
      if(getGyroZ() <= 0 && getGyroZ() > -90){beenInSecondQuadrant = true;}
      else if(getGyroZ() <= -90 && getGyroZ() > -180){beenInThirdQuadrant = true;}
      else if(getGyroZ() <= 180 && getGyroZ() > 90){beenInFourthQuadrant = true;}
      else if(getGyroZ() <= 90 && getGyroZ() > 0){beenInFirstQuadrant = true;}

      if(beenInFirstQuadrant && beenInSecondQuadrant && beenInThirdQuadrant && beenInFourthQuadrant){
        if(rotateLeftOrRight == LEFT){
          while(getGyroZ() > angleNow){
            //Serial.println(getGyroZ());
            move(rotateLeftOrRight, motorSpeed);
          }
          circleRotated = true;
        }
        else if(rotateLeftOrRight == RIGHT){
          while(getGyroZ() < angleNow){
            //Serial.println(getGyroZ());
            move(rotateLeftOrRight, motorSpeed);
          }
          circleRotated = true;
        }
      }

      move(rotateLeftOrRight, motorSpeed);
      //Serial.println(getGyroZ());
    }
  }
}

//Simple math (int truncates)
int calculateFullCirclesNeeded(int degreesToRotate){
  return abs(degreesToRotate)/360;
}

//Used when both motors should move with various speeds, used in conjuction with joystick steering
void moveBySeparateMotorSpeeds(int speedLeftMotor, int speedRightMotor){
  setEncoder1TarPWM(speedRightMotor);
  setEncoder2TarPWM(speedLeftMotor);
}

//Used for turning either right or left when in autonomous mode
direction_t randomLeftOrRight(){
  int leftOrRight = random(3, 5);

  if (leftOrRight == 3){
    return LEFT;
  }else{
    return RIGHT;
  }
}

//Simply make the robot stop OBS: may have to used continously to actually make the robot stop
void stopMotors(){
  resetStateLEDs();
  move(NONE, 0);
  setCurrentDirection(NONE);
}

void setCurrentDirection(direction_t newDirection){
  currentDirection = newDirection;
}

direction_t getCurrentDirection(){
  return currentDirection;
}

void setEncoderPwm(int encoderNumber, int pwmValue){
  //The in-built library is playing tricks with magic numbers: it removes a total of 2 in value if the speed is 2 or -2, or 1 if the value is 1 or -1 since it wants the motors to ramp up/down and have a safety margin of 2.
  if(encoderNumber == 1){
    if(pwmValue > 0){
      setEncoder1TarPWM(pwmValue + ENCODER_LIBRARY_PWM_OFFSET_VALUE);
    }
    else if(pwmValue < 0){
      setEncoder1TarPWM(pwmValue - ENCODER_LIBRARY_PWM_OFFSET_VALUE);
    }
    else{
      setEncoder1TarPWM(0);
    }
  }
  else if(encoderNumber == 2){
    if(pwmValue > 0){
      setEncoder2TarPWM(pwmValue + ENCODER_LIBRARY_PWM_OFFSET_VALUE);
    }
    else if(pwmValue < 0){
      setEncoder2TarPWM(pwmValue - ENCODER_LIBRARY_PWM_OFFSET_VALUE);
    }
    else{
      setEncoder2TarPWM(0);
    }
  }
}

void stopMotorsMS(int ms) {
  if(ms < 0.0){
    ms = 0.0;
  }
  long endTime = millis() + ms;
  while(millis() < endTime) stopMotors();
}

void _loop() {
  loopEncoders();
  updateGyro();
}



/*
 * 
 * DIAGNOSTIC CODE
 * 
 */


bool moveTest(direction_t robotDirection, int motorspeed)
{
  bool errorEncoutered = false;

  setCurrentDirection(robotDirection);
  int leftSpeed = 0;
  int rightSpeed = 0;

  if(getCurrentDirection() == FORWARD){

    activateManualForwardLEDs();
    leftSpeed = -motorspeed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = motorspeed;
  }
  else if(getCurrentDirection() == BACKWARD){

    activateManualBackwardLEDs();
    leftSpeed = motorspeed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -motorspeed;
  }
  else if(getCurrentDirection() == LEFT){

    activateManualLeftLEDs();
    leftSpeed = -motorspeed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -motorspeed;
  }
  else if(getCurrentDirection() == RIGHT){

    activateManualRightLEDs();
    leftSpeed = motorspeed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = motorspeed;
  }
  else if(getCurrentDirection() == NONE) {

    leftSpeed = 0;
    rightSpeed = 0;
  }
;
  driveTime(1000, getCurrentDirection(), motorspeed);

  if(getEncoder1CurPwm() != leftSpeed || getEncoder2CurPwm() != rightSpeed || getCurrentDirection() != robotDirection){
    errorEncoutered = true;
  }

  stopMotorsMS(1000);

  if(getEncoder1CurPwm() != 0 || getEncoder2CurPwm() != 0 || getCurrentDirection() != NONE){
    errorEncoutered = true;
  }

  return errorEncoutered;
}
