# telemetry.py
import random
import time

class Telemetry:
    def __init__(self, countdown=30):
        self.countdown = countdown  # start time in seconds
        self.real_data = {}
        self.fake_data = {}

    def generate_real(self):
        """Generate real telemetry with countdown decreasing each call."""
        self.real_data = {
            "Xcoord": round(random.uniform(100, 150), 2),
            "Ycoord": round(random.uniform(50, 100), 2),
            "Missile": random.choice(["Armed", "Unarmed"]),
            "TimeLeft(s)": max(0, self.countdown)
        }
        self.countdown -= 1
        return self.real_data

    def generate_fake(self):
        """Generate fake telemetry (example: frozen countdown at 99)."""
        self.fake_data = {
            "Xcoord": round(random.uniform(100, 150), 2),
            "Ycoord": round(random.uniform(50, 100), 2),
            "Missile": random.choice(["Armed", "Unarmed"]),
            "TimeLeft(s)": 99  # frozen misleading countdown
        }
        return self.fake_data

    def reset(self, countdown=30):
        """Reset countdown timer."""
        self.countdown = countdown


