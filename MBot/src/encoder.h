/**
 * @file encoder_functions.h
 * @brief Header file defining functions related to encoder management.
 */

#include "MeEncoderOnBoard.h"
#include "MePort.h"

#ifndef ENCODER_FUNCTIONS_H
#define ENCODER_FUNCTIONS_H

/**
 * @brief Sets up interrupts for encoder pulses.
 */
void setupEncoderInterrupts();

/**
 * @brief Processes the pulses generated by encoder 1.
 */
void isr_process_encoder1();

/**
 * @brief Processes the pulses generated by encoder 2.
 */
void isr_process_encoder2();

/**
 * @brief Prints the pulse values of both encoders.
 */
void printEncoderPulseValues();

/**
 * @brief Retrieves the pulse count of encoder 1.
 * @return The pulse count of encoder 1.
 */
long getEncoder1Pulses();

/**
 * @brief Retrieves the pulse count of encoder 2.
 * @return The pulse count of encoder 2.
 */
long getEncoder2Pulses();

/**
 * @brief Sets the pulse count of encoder 1 to a specified value.
 * @param pos The position to set for encoder 1.
 */
void setEncoder1Pulse(long pos);

/**
 * @brief Sets the pulse count of encoder 2 to a specified value.
 * @param pos The position to set for encoder 2.
 */
void setEncoder2Pulse(long pos);

/**
 * @brief Resets the pulse counts of both encoders.
 */
void resetEncoderValues();

/**
 * @brief Calculates the average pulse count between encoders.
 * @return The average pulse count between encoders.
 */
float getEncoderAverage();

/**
 * @brief Sets the target PWM value for encoder 1.
 * @param speedRightMotor The target PWM value for the right motor.
 */
void setEncoder1TarPWM(int16_t speedRightMotor);

/**
 * @brief Sets the target PWM value for encoder 2.
 * @param speedLeftMotor The target PWM value for the left motor.
 */
void setEncoder2TarPWM(int16_t speedLeftMotor);

/**
 * @brief Runs the loop function for both encoders.
 */
void loopEncoders();

/**
 * @brief Runs the loop function for encoder 1.
 */
void encoder1Loop();

/**
 * @brief Runs the loop function for encoder 2.
 */
void encoder2Loop();

/**
 * @brief Retrieves the current PWM value of encoder 1.
 * @return The current PWM value of encoder 1.
 */
int16_t getEncoder1CurPwm();

/**
 * @brief Retrieves the current PWM value of encoder 2.
 * @return The current PWM value of encoder 2.
 */
int16_t getEncoder2CurPwm();

#endif // ENCODER_FUNCTIONS_H
