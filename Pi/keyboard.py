from pynput import keyboard

class Keyboard:
    """Class to handle keyboard inputs."""

    def __init__(self, commands):
        """
        Initialize Keyboard object.
        
        Args:
        - commands (list): List of keys to track.
        """
        self.key_listener = keyboard.Listener(on_press=self.on_press, on_release=self.on_release)
        self.keys_to_track = {key: False for key in commands}

    def on_press(self, key):
        """
        Function triggered when a key is pressed.

        Args:
        - key: The pressed key.
        """
        try:
            char = key.char
            if char in self.keys_to_track:
                self.keys_to_track[char] = True
        except AttributeError:
            pass

    def on_release(self, key):
        """
        Function triggered when a key is released.

        Args:
        - key: The released key.
        """
        try:
            char = key.char
            if char in self.keys_to_track:
                self.keys_to_track[char] = False
        except AttributeError:
            pass

    def start_listen(self):
        """Start listening for keyboard events."""
        self.key_listener.start()

    def stop_listen(self):
        """Stop listening for keyboard events."""
        self.key_listener.stop()
