/**
 * @file led_functions.h
 * @brief Header file defining functions related to LED management.
 */

#include "MeRGBLed.h"

#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

/**
 * @brief This module contains various LED functions for indicating robot states (STANDBY or MANUAL).
 * LEDs are located on top of the Arduino board.
 */

/**
 * @brief Sets up the LED module.
 */
void setupLED();

/**
 * @brief Resets the state of LEDs.
 */
void resetStateLEDs();

/**
 * @brief Deactivates all LEDs.
 */
void deactivateLEDs();

/**
 * @brief Activates all RGB LEDs with a specified color and optional delay time.
 * @param r Red component value (0-255).
 * @param g Green component value (0-255).
 * @param b Blue component value (0-255).
 * @param delayTime Optional delay time between activations (default: 100 milliseconds).
 */
void activateAllLEDsRGB(int r, int g, int b, unsigned long delayTime = 100);

/**
 * @brief Activates LEDs to indicate the standby state.
 */
void activateStandbyLEDs();

/**
 * @brief Activates LEDs to indicate the manual state.
 */
void activateManualLEDs();

/**
 * @brief Activates LEDs to indicate manual forward movement.
 */
void activateManualForwardLEDs();

/**
 * @brief Activates LEDs to indicate manual backward movement.
 */
void activateManualBackwardLEDs();

/**
 * @brief Activates LEDs to indicate manual right movement.
 */
void activateManualRightLEDs();

/**
 * @brief Activates LEDs to indicate manual left movement.
 */
void activateManualLeftLEDs();

/**
 * @brief Deactivates LEDs and performs a test.
 * @return Returns the result of the deactivation test.
 */
int deactivateLEDsTest();

/**
 * @brief Activates standby LEDs and performs a test.
 * @return Returns the result of the standby LED activation test.
 */
int activateStandbyLEDsTest();

#endif // LED_FUNCTIONS_H
