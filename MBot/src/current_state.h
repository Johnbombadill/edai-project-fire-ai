/**
 * @file current_state.h
 * @brief Header file defining functions related to robot state management.
 */

#ifndef CURRENT_STATE_H
#define CURRENT_STATE_H

/**
 * @brief Enum representing different states of the robot.
 */
typedef enum {
    STANDBY, /**< Robot is in standby state */
    MANUAL /**< Robot is in manual mode */
} robotState_t;

/**
 * @brief Retrieves the current state of the robot.
 * @return The current state of the robot.
 */
robotState_t getCurrentState();

/**
 * @brief Sets the new state of the robot.
 * @param newState The new state to be set for the robot.
 */
void setCurrentState(robotState_t newState);

#endif // CURRENT_STATE_H
