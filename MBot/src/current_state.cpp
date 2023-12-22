#include "current_state.h"

robotState_t currentState;

robotState_t getCurrentState(){
  return currentState;
}

void setCurrentState(robotState_t newState){
  currentState = newState;
}