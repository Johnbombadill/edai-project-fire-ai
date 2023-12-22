/**
 * @file motor_control.h
 * @brief Header file containing functions related to motor control.
 */

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

/**
 * @brief All functions handling motor control and operations are defined in this module.
 * Basic operations such as moving in specific directions, as well as more complex procedures like moving specific distances or durations, are included.
 * Rotation functions are also available here.
 */

typedef enum {
  NONE, /**< No movement */
  FORWARD, /**< Movement forward */
  BACKWARD, /**< Movement backward */
  LEFT, /**< Movement left */
  RIGHT /**< Movement right */
} direction_t;

/**
 * @brief Sets up motor-related configurations.
 */
void setupMotors();

/**
 * @brief Performs a tick for manual control operations.
 */
void doManualControlTick();

/**
 * @brief Moves the robot in a specified direction at a particular speed.
 * @param direction The direction to move in.
 * @param speedVal The speed value for movement.
 */
void move(direction_t direction, float speedVal);

// ... (Other function declarations)

/**
 * @brief Stops the motors.
 */
void stopMotors();

/**
 * @brief Sets the current direction of the robot.
 * @param newDirection The new direction to set.
 */
void setCurrentDirection(direction_t newDirection);

/**
 * @brief Retrieves the current direction of the robot.
 * @return The current direction of the robot.
 */
direction_t getCurrentDirection();

/**
 * @brief Sets the PWM value for an encoder.
 * @param encoderNumber The encoder number.
 * @param pwmValue The PWM value to set.
 */
void setEncoderPwm(int encoderNumber, int pwmValue);

/**
 * @brief Stops the motors for a specified duration.
 * @param ms The duration in milliseconds to stop the motors.
 */
void stopMotorsMS(int ms);

/**
 * @brief Internal loop function for motor control operations.
 */
void _loop();

/*
 * DIAGNOSTIC CODE
 */
 
/**
 * @brief Performs a diagnostic movement test.
 * @param robotDirection The direction for the test.
 * @param motorspeed The motor speed for the test.
 * @return True if the test is successful, otherwise false.
 */
bool moveTest(direction_t robotDirection, int motorspeed);

#endif // MOTOR_CONTROL_H
