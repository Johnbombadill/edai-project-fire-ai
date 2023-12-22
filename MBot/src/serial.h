/**
 * @file serial_communication.h
 * @brief Header file containing functions related to serial communication with the Pi.
 */

#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

/**
 * @brief This module handles communication to and from the Pi via serial communication.
 * It periodically reads the serial bus, stores received data, and performs appropriate actions based on the received data.
 */

#include <ArduinoQueue.h>
#include <WString.h>
#include "config.h"

/**
 * @brief Enum defining different types of messages received.
 */
typedef enum {
  Hello, /**< Hello message received */
  Standby, /**< Standby mode message received */
  ManualStop, /**< Manual stop message received */
  ManualForward, /**< Manual forward movement message received */
  ManualBackward, /**< Manual backward movement message received */
  ManualLeft, /**< Manual left movement message received */
  ManualRight, /**< Manual right movement message received */
  SetManualMotorSpeedHigh, /**< Set manual motor speed to high message received */
  SetManualMotorSpeedMedium, /**< Set manual motor speed to medium message received */
  SetManualMotorSpeedLow, /**< Set manual motor speed to low message received */
  Error /**< Error message received */
} messageRecieved_t;

/**
 * @brief Sets up the serial communication.
 */
void setupSerial();

/**
 * @brief Sends temperature transmission data over serial.
 * @param currentTemperature The current temperature to send.
 */
void sendTemperatureTransmission(int currentTemperature);

/**
 * @brief Performs a tick for serial communication operations.
 */
void doSerialTick();

/**
 * @brief Reads data from the serial bus.
 */
void readSerialData();

/**
 * @brief Sends an acknowledgment message.
 * @param message The message to acknowledge.
 */
void ackMessage(String message);

/**
 * @brief Reads data from the serial bus and returns it.
 * @return The data read from the serial bus.
 */
String readSerialBus();

/**
 * @brief Sets received serial data to store.
 * @param s_dataToStore The data to store.
 */
void setSerialDataRecieved(String s_dataToStore);

/**
 * @brief Retrieves the stored received serial data.
 * @return The stored received serial data.
 */
String getSerialDataRecieved();

/**
 * @brief Sends a message indicating failure.
 * @param message The failure message to send.
 */
void sendMessageNOK(String message);

/**
 * @brief Sends an acknowledgment message.
 * @param message The acknowledgment message to send.
 */
void sendMessageAck(String message);

/**
 * @brief Sends a signal indicating the ultrasonic sensor was triggered.
 */
void sendSerialUltraSonicTriggered();

/**
 * @brief Sends coordinates over serial.
 */
void sendSerialCoordinates();

/**
 * @brief Sends a diagnostic success signal.
 */
void sendDiagnosticSuccess();

/**
 * @brief Sends a diagnostic failure signal with fault codes.
 * @param faultCodes The fault codes to send.
 */
void sendDiagnosticFailed(ArduinoQueue<int> faultCodes);

/**
 * @brief Clears stored messages.
 */
void clearStoredMessages();

/**
 * @brief Checks if a capture acknowledgment was received.
 * @return True if a capture acknowledgment was received, otherwise false.
 */
bool recievedCaptureAck();

/**
 * @brief Checks if an acknowledgment was received for a message.
 * @return True if an acknowledgment was received, otherwise false.
 */
bool ackReviecedMessage();

/**
 * @brief Converts a message string to its corresponding enum value.
 * @param message The message string to convert.
 * @return The corresponding enum value of the message.
 */
messageRecieved_t convertMessageToInt(String message);

#endif // SERIAL_COMMUNICATION_H
