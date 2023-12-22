import paho.mqtt.client as mqtt
import time
import numpy as np
import datetime
import subprocess
import os
import psutil

import settings


class MQTTClient:
    """Class to handle MQTT client operations."""

    def __init__(self, mqtt_id):
        """
        Initialize MQTTClient object.

        Args:
        - mqtt_id (str): MQTT client ID.
        """
        self.mqtt_id = mqtt_id
        self.client = mqtt.Client(self.mqtt_id, protocol=mqtt.MQTTv31)

        self.client.on_message = self.on_message

        self.latest_payload = None
        self.latest_payload_topic = None
        self.new_payload_available = False

    def on_message(self, client, userdata, message):
        """
        Callback function triggered on receiving a message.

        Args:
        - client: The client instance for this callback.
        - userdata: The private user data.
        - message: The received message.
        """
        self.latest_payload = message.payload
        self.latest_payload_topic = message.topic
        self.new_payload_available = True

    def print_message_details(self, message):
        """
        Print details of the received message.

        Args:
        - message: The message to print details for.
        """
        print("\n\n*********************************************************")
        print(f"\tmessage payload: {message.payload.decode('utf-8')}")
        print(f"\tmessage topic: {message.topic}")
        print(f"\tmessage qos: {message.qos}")
        print(f"\tmessage retain flag: {message.retain}")
        print("*********************************************************\n\n")

    def connect(self, broker_address, port=1883):
        """
        Connect to the MQTT broker.

        Args:
        - broker_address (str): Address of the MQTT broker.
        - port (int, optional): Port number for the connection. Defaults to 1883.
        """
        print("\nConnecting to broker:", broker_address)
        self.client.connect(broker_address, port)
        self.start_client()

    def start_client(self):
        """Start the MQTT client."""
        print("\nStarting client")
        self.client.loop_start()

    def subscribe(self, topic, qos=2):
        """
        Subscribe to an MQTT topic.

        Args:
        - topic (str): Topic to subscribe to.
        - qos (int, optional): Quality of Service. Defaults to 2.
        """
        print("\nSubscribing to topic:", topic)
        self.client.subscribe(topic, qos=qos)

    def unsubscribe(self, topic):
        """
        Unsubscribe from an MQTT topic.

        Args:
        - topic (str): Topic to unsubscribe from.
        """
        print("\nUnsubscribing from topic:", topic)
        self.client.unsubscribe(topic)

    def publish(self, topic, message, qos=2):
        """
        Publish a message to an MQTT topic.

        Args:
        - topic (str): Topic to publish to.
        - message: Message to publish.
        - qos (int, optional): Quality of Service. Defaults to 2.
        """
        #print("\nPublishing message", message, "to topic:", topic) # Uncomment if you wish to see all data being published
        self.client.publish(topic, message, qos=qos)

    def stop_client(self):
        """Stop the MQTT client."""
        print("\nStopping client")
        self.client.loop_stop()

    def disconnect(self):
        """Disconnect from the MQTT broker."""
        print("\nDisconnecting from broker")
        self.client.disconnect()

    def get_new_payload(self):
        """
        Get the latest payload received.

        Returns:
        - latest_payload: The latest payload.
        """
        self.new_payload_available = False
        return self.latest_payload

    def get_new_payload_available_and_what_topic(self):
        """
        Check if a new payload is available and its topic.

        Returns:
        - new_payload_available (bool): Whether a new payload is available.
        - latest_payload_topic: The topic of the latest payload.
        """
        return self.new_payload_available, self.latest_payload_topic

    def is_mosquitto_running(self):
        """
        Check if the Mosquitto MQTT broker is running.

        Returns:
        - bool: True if Mosquitto is running, False otherwise.
        """
        for proc in psutil.process_iter(['pid', 'name']):
            if proc.info['name'] == 'mosquitto.exe':
                return True
        return False

    def start_mosquitto_server(self):
        """Start the Mosquitto MQTT broker server if not already running."""
        if not self.is_mosquitto_running():
            current_directory = os.path.dirname(os.path.abspath(__file__))
            mosquitto_exe_path = os.path.join(current_directory, "mosquitto", "mosquitto.exe")
            subprocess.Popen(mosquitto_exe_path)

def run_mqtt_subscriber(mqtt_client):
    """
    Run MQTT subscriber.

    Args:
    - mqtt_client: The MQTTClient instance.
    """
    try:
        mqtt_client.connect(broker_address=settings.BROKER_ADRESS, port=settings.BROKER_PORT)
    except Exception as e:
        print("Exception MQTT Client:", e)

    try:
        mqtt_client.subscribe(topic=settings.TOPIC_ROBOT_STATE)
        mqtt_client.subscribe(topic=settings.TOPIC_MOTOR_CONTROL_SPEED)
        mqtt_client.subscribe(topic=settings.TOPIC_MOTOR_CONTROL_DIRECTION)
    except Exception as e:
        print("Exception MQTT Client:", e)

    while True:
        time.sleep(settings.MQTT_THREAD_SLEEP_TIME_IN_SECONDS)
