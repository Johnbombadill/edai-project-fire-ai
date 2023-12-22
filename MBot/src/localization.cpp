#include "config.h"
#include "serial.h"
#include "gyro.h"
#include "encoder.h"
#include "localization.h"

int coordinateX = 0;
int coordinateY = 0;

//We only want to update the Pi with coordinates with a certain interval, storing the last time we updated is done in a variable
long timeForNextLocationTick = 0;

//Tick that checks whether we should update our coordinates or wait until the timer has reset
void doLocalizationTick(){
  if(millis() > timeForNextLocationTick){
    timeForNextLocationTick = millis() + AUTONOMOUS_LOCATION_TICK_TIME_MS;

    calculateAndUpdateXAndYCoordinates();

    sendSerialCoordinates();
  }
}

//This function calculates what the coordinates should be when called and sets them in the global variables
void calculateAndUpdateXAndYCoordinates(){
  float newXCoordinate;
  float newYCoordinate;
  
  setGyroValueAtEnd(getGyroZ());
  
  float calcTemp = ((getAverageGyroValue() + LOCALIZATION_CIRCLE_ROTATION_OFFSET) * DEGREES_TO_RADIAN_FACTOR);

  newYCoordinate = getCoordinateY() + (getDistanceTravelled() * (sin(calcTemp)) *-1); //*-1 due to Canvas-inversion in app
  newXCoordinate = getCoordinateX() + (getDistanceTravelled() * (cos(calcTemp) *-1)); //same as above

  setCoordinateX(newXCoordinate);
  setCoordinateY(newYCoordinate);

  resetEncoderValues();
}

//Knowing how many pulses are generated per millimeter makes sure that the calculation is correct even when battery is low/robot moving slow
float getDistanceTravelled(){
  return (getEncoderAverage() * MILLIMETER_PER_ENCOER_PULSE);
}

void printCoordinates(){
  Serial.println("Coordinate X: " + String(getCoordinateX()));
  Serial.println("Coordinate Y: " + String(getCoordinateY()) + "\n");
}

//Following functions gets, sets or resets the coorinates
int getCoordinateX(){
  return coordinateX;
}

int getCoordinateY(){
  return coordinateY;
}

void setCoordinateX(int value){
  coordinateX = value;
}

void setCoordinateY(int value){
  coordinateY = value;
}

void resetCoorinateX(){
  coordinateX = 0;
}

void resetCoorinateY(){
  coordinateY = 0;
}

void resetCoordinates(){
  resetCoorinateX();
  resetCoorinateY();
}
