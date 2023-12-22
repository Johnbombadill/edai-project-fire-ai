#include "localization.h"
#include "led.h"
#include "current_state.h"

MeRGBLed rgbled_0(0, 12);
float standby_brightness = 0.0;
bool standby_brightness_increase = true;

void setupLED(){
  rgbled_0.setpin(44);
}

void resetStateLEDs(){
  switch(getCurrentState()){
    case(STANDBY):
      activateStandbyLEDs();
      break;
    case(MANUAL):
      activateManualLEDs();
      break;
  }
}

void deactivateLEDs(){
  rgbled_0.setColor(0, 0, 0, 0);
  rgbled_0.show();
}

void activateAllLEDsRGB(int r, int g, int b, unsigned long delayTime = 100){
  rgbled_0.setColor(0, r, g, b);
  rgbled_0.show();
  delay(delayTime);
}

void activateStandbyLEDs() {
  if (getCurrentState() == STANDBY) {
    if (standby_brightness_increase && standby_brightness < 100.0) {
      standby_brightness += 0.1;
    } else if (!standby_brightness_increase && standby_brightness > 0.0) {
      standby_brightness -= 0.1;
    }

    // Toggle direction if brightness reaches limits
    if (standby_brightness >= 100.0) {
      standby_brightness_increase = false;
    } else if (standby_brightness <= 0.0) {
      standby_brightness_increase = true;
    }

    int brightnessInt = int(standby_brightness); // Adjust the scaling factor as needed

    rgbled_0.setColor(0, 0, 0, brightnessInt);
    rgbled_0.show();
  }
}

void activateManualLEDs(){
  rgbled_0.setColor(0, 100, 100, 0);
  rgbled_0.show();
}

void activateManualForwardLEDs(){
  rgbled_0.setColor(2, 100, 0, 0);
  rgbled_0.setColor(3, 100, 0, 0);
  rgbled_0.setColor(4, 100, 0, 0);
  rgbled_0.show();
}

void activateManualBackwardLEDs(){
  rgbled_0.setColor(8, 100, 0, 0);
  rgbled_0.setColor(9, 100, 0, 0);
  rgbled_0.setColor(10, 100, 0, 0);
  rgbled_0.show();
}

void activateManualRightLEDs(){
  rgbled_0.setColor(5, 100, 0, 0);
  rgbled_0.setColor(6, 100, 0, 0);
  rgbled_0.setColor(7, 100, 0, 0);
  rgbled_0.show();
}

void activateManualLeftLEDs(){
  rgbled_0.setColor(11, 100, 0, 0);
  rgbled_0.setColor(12, 100, 0, 0);
  rgbled_0.setColor(1, 100, 0, 0);
  rgbled_0.show();
}



/*
 * 
 * DIAGNOSTIC CODE
 * 
 */


int deactivateLEDsTest(){
  int errorCounter = 0;

  if(!rgbled_0.setColor(0, 0, 0, 0)){errorCounter ++;}
  
  rgbled_0.show();

  return errorCounter;
}

int activateStandbyLEDsTest(){
  int errorCounter = 0;
  
  if(!rgbled_0.setColor(0, 0, 0, 100)){errorCounter ++;}
  
  rgbled_0.show();

  return errorCounter;
}
