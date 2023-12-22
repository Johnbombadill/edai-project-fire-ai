import paho.mqtt.client as mqtt
import time
import numpy as np
import datetime
import threading
import cv2

from mqtt_client import MQTTClient
import settings
from keyboard import Keyboard
from slam import Slam

def flatten(lst):
    """
    Flatten a nested list.

    Args:
    - lst (list): Nested list to be flattened.

    Returns:
    - flat_list (list): Flattened list.
    """
    flat_list = []
    for item in lst:
        if isinstance(item, list):
            flat_list.extend(flatten(item))
        else:
            flat_list.append(item)
    return flat_list

def determine_topic_from_keys(pressed_keys):
    """
    Determine MQTT topic from pressed keys.

    Args:
    - pressed_keys (list): List of pressed keys.

    Returns:
    - str or None: MQTT topic corresponding to the keys, or None if no match is found.
    """
    if not pressed_keys:
        return None

    serial_input = "".join(pressed_keys)

    if serial_input in flatten(settings.STATE_COMMANDS):
        return settings.TOPIC_ROBOT_STATE
    elif serial_input in flatten(settings.MOTOR_SPEED_COMMANDS):
        return settings.TOPIC_MOTOR_CONTROL_SPEED
    elif serial_input in flatten(settings.MOTOR_DIRECTION_COMMANDS):
        return settings.TOPIC_MOTOR_CONTROL_DIRECTION

    return None  # Return None if the keys don't match any known command

def main():
    """
    Main function to control various components and perform actions based on MQTT topics and keyboard inputs.
    """
    # Instantiation
    keyboard_instance = Keyboard(flatten(settings.ALL_COMMANDS))
    keyboard_instance.start_listen()
    slam_client = Slam()
    mqtt_client = MQTTClient(settings.CONTROLLER_CLIENT_ID)
    
    # Start local mosquitto server if set in settings
    if settings.USE_LOCAL_MOSQUITTO_SERVER:
        try:
            mqtt_client.start_mosquitto_server()
        except Exception as e:
            print("Exception MQTT Client:", e)
            return False

    # Connect to MQTT broker
    try:
        mqtt_client.connect(settings.BROKER_ADRESS, port=settings.BROKER_PORT)
    except Exception as e:
        print(f"\nConnection error: {e}\n")

    # Subscribe to MQTT topics
    try:
        mqtt_client.subscribe(settings.TOPIC_CAMERA_DATA)
        mqtt_client.subscribe(settings.TOPIC_TEMPERATURE_DATA)
    except Exception as e:
        print(f"\Subscription error: {e}\n")

    # Main loop
    current_temperature = None
    last_frame_time = time.time()
    current_temperature = None

    while True:
        # MQTT Payload
        new_payload_available, topic = mqtt_client.get_new_payload_available_and_what_topic()
        if new_payload_available:
            if topic == settings.TOPIC_CAMERA_DATA:
                # Frame visualization
                payload = mqtt_client.get_new_payload()
                img = cv2.imdecode(np.frombuffer(payload, np.uint8), cv2.IMREAD_COLOR)
                cv2.imshow('Frame', img)
                cv2.waitKey(1)  # Add a short delay to update the imshow window
                last_frame_time = time.time()  # Update the time when the frame was received

                # Slam visualization 
                slam_client.do_slam(img)

            elif topic == settings.TOPIC_SLAM_DATA and settings.RUN_SLAM_ON_PI:
                payload = mqtt_client.get_new_payload()
                slam_client.setSlamData(payload)
                slam_client.visualizeSlamData()
            elif topic == settings.TOPIC_TEMPERATURE_DATA:
                payload = mqtt_client.get_new_payload()
                current_temperature = int(payload)
                print("\nCurrent temperature:", current_temperature) # Temperature not used more than this
        
        if time.time() - last_frame_time > settings.CAMERA_DATA_INTERVAL_MAX_TIME_IN_SECONDS:
            cv2.destroyAllWindows()
            slam_client.pointcloud_window_created = False

        # MQTT Button presses
        pressed_keys = [key for key, value in keyboard_instance.keys_to_track.items() if value]
        if not any(pressed_keys):
            continue
        topic = determine_topic_from_keys(pressed_keys)
        if topic:
            for key, value in keyboard_instance.keys_to_track.items():
                if value:
                    try:
                        mqtt_client.publish(topic, key)
                        print("PUB key:", key, " - to:", topic)
                    except Exception as e:
                        print(f"Publish error: {e}")

        # Sleep
        time.sleep(settings.PUBLISHER_THREAD_SLEEP_TIME_IN_SECONDS)

if __name__ == "__main__":
    main()
