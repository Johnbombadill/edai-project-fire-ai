import argparse
import threading
import time

from mqtt_client import MQTTClient, run_mqtt_subscriber
from camera import PiCamera, run_camera
from serial_communication import SerialCommunication, run_serial
import settings

def parse_arguments():
    """
    Parse command-line arguments.

    Returns:
    - args: Parsed arguments from the command line.
    """
    parser = argparse.ArgumentParser(description="Control camera thread")
    parser.add_argument("--camera-off", action="store_true", help="Turn off the camera thread")
    return parser.parse_args()

if __name__ == "__main__":
    """
    Entry point of the script.
    """
    # Argument parsing
    args = parse_arguments()

    # Instantiation
    my_camera = PiCamera() if not args.camera_off else None
    mqtt_client = MQTTClient(mqtt_id=settings.RPI_CLIENT_ID)

    # Thread creations
    mqtt_thread = threading.Thread(target=run_mqtt_subscriber, args=[mqtt_client])
    camera_thread = threading.Thread(target=run_camera, args=[my_camera, mqtt_client]) if not args.camera_off else None
    serial_thread = threading.Thread(target=run_serial, args=[mqtt_client])

    # Thread starts
    mqtt_thread.start()
    if not args.camera_off:
        camera_thread.start()
    serial_thread.start()

    # Main loop
    try:
        while True:
            time.sleep(settings.MAIN_THREAD_SLEEP_TIME_SECONDS)  # Keep the main thread alive
    except KeyboardInterrupt:
        pass  # Placeholder for cleanup actions
