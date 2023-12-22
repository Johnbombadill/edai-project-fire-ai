/**
 * @file dead_reckoning.h
 * @brief Header file containing functions for dead reckoning functionality.
 */

#ifndef DEAD_RECKONING_H
#define DEAD_RECKONING_H

/**
 * @brief All dead-reckoning functionality resides within this module.
 * The process relies on encoder updates from the motors to track wheel movement.
 * Position determination is based on these encoder values, offering millimeter accuracy for this robot.
 * However, prolonged use may result in position deviation.
 * Complementary GPS/GNSS, better encoders, or alternative methods to validate and calibrate encoder values can address this issue.
 */

/**
 * @brief All code in this module is attributed to RNR12.
 */

#include <Arduino.h>

/**
 * @brief Executes a tick in the localization process.
 */
void doLocalizationTick();

/**
 * @brief Calculates and updates the X and Y coordinates based on encoder values.
 */
void calculateAndUpdateXAndYCoordinates();

/**
 * @brief Retrieves the distance traveled based on encoder values.
 * @return The distance traveled.
 */
float getDistanceTravelled();

/**
 * @brief Prints the current coordinates to the Serial output.
 */
void printCoordinates();

/**
 * @brief Retrieves the current X coordinate.
 * @return The X coordinate.
 */
int getCoordinateX();

/**
 * @brief Retrieves the current Y coordinate.
 * @return The Y coordinate.
 */
int getCoordinateY();

/**
 * @brief Sets the X coordinate to a specified value.
 * @param value The value to set the X coordinate to.
 */
void setCoordinateX(int value);

/**
 * @brief Sets the Y coordinate to a specified value.
 * @param value The value to set the Y coordinate to.
 */
void setCoordinateY(int value);

/**
 * @brief Resets the X coordinate to zero.
 */
void resetCoordinateX();

/**
 * @brief Resets the Y coordinate to zero.
 */
void resetCoordinateY();

/**
 * @brief Resets both X and Y coordinates to zero.
 */
void resetCoordinates();

#endif // DEAD_RECKONING_H
