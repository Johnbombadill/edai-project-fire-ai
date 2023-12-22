import cv2
import time

import settings

class PiCamera:
    """Class to handle operations related to a Raspberry Pi camera."""

    def __init__(self):
        """Initialize PiCamera object."""
        self.camera = cv2.VideoCapture(0)  # Use 0 for the first camera
        self.camera.set(cv2.CAP_PROP_FRAME_WIDTH, settings.RESOLUTION_VGA[0])
        self.camera.set(cv2.CAP_PROP_FRAME_HEIGHT, settings.RESOLUTION_VGA[1])
        self.latest_frame = None 
        self.new_frame_ready = False

    def update_frame(self):
        """Update the camera frame."""
        ret, frame = self.camera.read()
        if ret:
            frame = cv2.flip(frame, -1)  # Flips both vertically and horizontally
            
            _, jpeg_frame = cv2.imencode('.jpg', frame)
            self.latest_frame = jpeg_frame.tobytes()
            self.new_frame_ready = True
        else:
            self.new_frame_ready = False

    def get_latest_frame(self):
        """Get the latest camera frame."""
        self.new_frame_ready = False
        return self.latest_frame

    def is_new_frame_ready(self):
        """Check if a new frame is ready."""
        return self.new_frame_ready

    def close(self):
        """Release the camera resources."""
        self.camera.release()

def run_camera(camera, mqtt_client):
    """Run the camera continuously and publish frames via MQTT."""
    while True:
        start_time = time.time()
        camera.update_frame()

        if camera.is_new_frame_ready():
            frame = camera.get_latest_frame()
            if frame:
                mqtt_client.publish(settings.TOPIC_CAMERA_DATA, frame)

        elapsed_time = time.time() - start_time
        sleep_time = max(0, settings.CAMERA_THREAD_SLEEP_TIME_IN_SECONDS - elapsed_time)
        time.sleep(sleep_time)
