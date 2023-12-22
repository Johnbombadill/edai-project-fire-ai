/**
 * @file gyro_functions.h
 * @brief Header file defining functions related to gyroscopic sensor management.
 */

#include "MeGyro.h"

#ifndef GYRO_FUNCTIONS_H
#define GYRO_FUNCTIONS_H

/**
 * @brief Sets up the gyro sensor.
 */
void setupGyro();

/**
 * @brief Retrieves the X-axis value from the gyro sensor.
 * @return The X-axis value from the gyro sensor.
 */
float getGyroX();

/**
 * @brief Retrieves the Y-axis value from the gyro sensor.
 * @return The Y-axis value from the gyro sensor.
 */
float getGyroY();

/**
 * @brief Retrieves the Z-axis value from the gyro sensor.
 * @return The Z-axis value from the gyro sensor.
 */
float getGyroZ();

/**
 * @brief Prints the values of the gyro sensor.
 */
void gyroPrintValues();

/**
 * @brief Sets the initial value for the gyro sensor at the start.
 * @param value The value to set at the start.
 */
void setGyroValueAtStart(float value);

/**
 * @brief Sets the final value for the gyro sensor at the end.
 * @param value The value to set at the end.
 */
void setGyroValueAtEnd(float value);

/**
 * @brief Resets the initial value for the gyro sensor.
 */
void resetGyroValueAtStart();

/**
 * @brief Resets the final value for the gyro sensor.
 */
void resetGyroValueAtEnd();

/**
 * @brief Resets both the initial and final values for the gyro sensor.
 */
void resetGyroStartAndEnd();

/**
 * @brief Retrieves the initial value set for the gyro sensor.
 * @return The initial value set for the gyro sensor.
 */
float getGyroValueAtStart();

/**
 * @brief Retrieves the final value set for the gyro sensor.
 * @return The final value set for the gyro sensor.
 */
float getGyroValueAtEnd();

/**
 * @brief Calculates and retrieves the average value from the gyro sensor.
 * @return The average value from the gyro sensor.
 */
float getAverageGyroValue();

/**
 * @brief Updates the gyro sensor values.
 */
void updateGyro();

#endif // GYRO_FUNCTIONS_H
