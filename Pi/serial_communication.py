import serial
import time
import settings

class SerialCommunication:
    """Class to handle serial communication."""

    def __init__(self, port, baudrate=57600, timeout=1):
        """
        Initialize SerialCommunication object.

        Args:
        - port (str): Port for serial communication.
        - baudrate (int, optional): Baudrate for serial communication. Defaults to 57600.
        - timeout (int, optional): Timeout for serial communication. Defaults to 1.
        """
        self.serial_port = serial.Serial(port, baudrate, timeout=timeout)
        self.sent_commands = []
        self.unack_counter = 0   # Counter for unacknowledged commands

    def send_command(self, command):
        """
        Send a command over serial communication.

        Args:
        - command (str): Command to be sent.

        Returns:
        - response (str): Response received after sending the command.
        """
        self.serial_port.write(command.encode())
        time.sleep(0.1)
        response = self.serial_port.read_until(b'!').decode().rstrip('!')  # Wait for '!' and remove it from response
        if response == '':
            response = self.serial_port.read_until(b'?').decode().rstrip('?')  # Wait for '?' and remove it from response
        
        self.sent_commands.append(command) # Store sent command in the list

        return response

def show_that_connection_to_mbot_is_set(serial_comm):
    """
    Show a connection indication to mBot via serial communication.

    Args:
    - serial_comm (SerialCommunication): SerialCommunication instance.
    """
    set_motor_speed_medium = "m\n" # Will make the robot blink white once - used as indicator for that the RPI is up and running when no monitor connected
    serial_comm.send_command(set_motor_speed_medium)
    time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
    serial_comm.send_command(set_motor_speed_medium)
    time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
    serial_comm.send_command(set_motor_speed_medium)
    time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
    serial_comm.send_command(set_motor_speed_medium)
    time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
    serial_comm.send_command(set_motor_speed_medium)
    time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
    serial_comm.send_command(set_motor_speed_medium)
    time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
    serial_comm.send_command(set_motor_speed_medium)

def run_serial(mqtt_client):
    """
    Run the serial communication thread.

    Args:
    - mqtt_client (MQTTClient): MQTTClient instance.
    """
    try:
        serial_comm = SerialCommunication(port=settings.RPI_USB_PORT)
        show_that_connection_to_mbot_is_set(serial_comm)
    except Exception as e:
        # If the RPI serial port is not found, try with a different port (e.g., COM3, may be on windows)
        try:
            print("Exception: ", e)
            serial_comm = SerialCommunication(port=settings.WIN_USB_PORT)
            show_that_connection_to_mbot_is_set(serial_comm)
        except Exception as e2:
            print("Exception: ", e2)
            return  # Exit the function if no serial port is available

    last_temperature_transmission_time = time.time()  # Initialize last execution time

    # Main loop of thread
    while True:
        new_data_is_available, topic = mqtt_client.get_new_payload_available_and_what_topic()
        if new_data_is_available and (topic == settings.TOPIC_MOTOR_CONTROL_DIRECTION or topic == settings.TOPIC_MOTOR_CONTROL_SPEED or topic == settings.TOPIC_ROBOT_STATE):
            payload = mqtt_client.get_new_payload()
            command = payload.decode('utf-8') + '\n'
            response = serial_comm.send_command(command)

        command_received = None

        if serial_comm.serial_port.in_waiting > 0:
            command_received = serial_comm.serial_port.readline().decode().rstrip()  # Read the command from serial input

        # We received a message
        if command_received is not None:
            if '!' in command_received:
                # Message acknowledged, remove from the sent list
                ack_command = command_received.split('!')[0]  # Extract acknowledged command
                if ack_command in serial_comm.sent_commands:
                    serial_comm.sent_commands.remove(ack_command)
            elif '?' in command_received:
                # Message not acknowledged, increment unack_counter
                unack_command = command_received.split('?')[0]  # Extract unacknowledged command
                if unack_command in serial_comm.sent_commands:
                    serial_comm.sent_commands.remove(unack_command)
                    serial_comm.unack_counter += 1
            else:
                if settings.TEMPERATURE_COMMAND in command_received:
                    current_time = time.time()
                    if (current_time - last_temperature_transmission_time) >= settings.TEMPERATURE_UPDATE_INTERVAL_SECONDS:
                        last_temperature_transmission_time = current_time
                        parts = command_received.split(':')
                        if len(parts) == 2:
                            value = parts[1].strip()  # Extract the value after "t:"
                            if value.isdigit():
                                temperature_value = int(value)
                                try:
                                    mqtt_client.publish(settings.TOPIC_TEMPERATURE_DATA, temperature_value)
                                    print("PUB temp:",temperature_value, " - to:",settings.TOPIC_TEMPERATURE_DATA)
                                except Exception as e:
                                    print(f"Publish error: {e}")
                            
                elif command_received.strip(): # May happen that an empty message is read somehow
                    print("\nMessage received but not recognized:", command_received, "\n")

        # Sleep
        time.sleep(settings.SERIAL_THREAD_SLEEP_TIME_IN_SECONDS)
