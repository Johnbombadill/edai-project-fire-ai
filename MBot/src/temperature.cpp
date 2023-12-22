#include "MeOnBoardTemp.h"

MeOnBoardTemp tempSensor;

int getCurrentTemperature(){
    return tempSensor.readValue();
}