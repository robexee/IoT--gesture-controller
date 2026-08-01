"""
camera.py

Provides FingerIdentifier: detects a single hand in a video frame
with MediaPipe and identifies which one finger (Thumb, Index, Middle,
Ring, or Pinky) is currently raised. A short rolling buffer smooths
the raw per-frame detection so the reported finger doesn't flicker
between frames.

This module only defines the class and its helpers — it does not
open a camera or run a loop itself. See main.py for the entry point
that drives this class from a live webcam feed.
"""

from collections import deque, Counter

import cv2 as cv
import mediapipe as mp

FINGER_NAMES = ("Thumb", "Index", "Middle", "Ring", "Pinky")


class MajorityVoteSmoother:
    """Smooths a stream of noisy values into the most frequent one seen in a short rolling window."""

    def __init__(self, window_size: int = 5):
        self._buffer = deque(maxlen=window_size)

    def push(self, value):
        """Adds a value and returns the current smoothed (most common) value."""
        self._buffer.append(value)
        return Counter(self._buffer).most_common(1)[0][0]

    def reset(self):
        self._buffer.clear()


class FingerIdentifier:
    """Detects a hand in a frame and identifies which single finger is raised."""

    def __init__(self, detection_conf: float = 0.5, tracking_conf: float = 0.5, buffer_size: int = 5):
        self._mp_drawing = mp.solutions.drawing_utils
        self._mp_hands = mp.solutions.hands
        self._hands = self._mp_hands.Hands(
            max_num_hands=1,
            min_detection_confidence=detection_conf,
            min_tracking_confidence=tracking_conf,
        )
        self._landmark_style = self._mp_drawing.DrawingSpec(color=(0, 255, 0), thickness=3, circle_radius=1)
        self._smoother = MajorityVoteSmoother(buffer_size)

    def process(self, rgb_frame):
        """Runs hand detection on an RGB frame; returns the raw MediaPipe results."""
        return self._hands.process(rgb_frame)

    def draw_landmarks(self, frame, hand_landmarks):
        self._mp_drawing.draw_landmarks(
            frame, hand_landmarks, self._mp_hands.HAND_CONNECTIONS, self._landmark_style
        )

    def identify_finger(self, hand_landmarks, hand_label: str):
        """Returns the smoothed name of the single raised finger, or None."""
        raised = self._raised_fingers(hand_landmarks, hand_label)
        return self._smoother.push(self._name_of_raised(raised))

    def reset(self):
        """Clears the smoothing buffer, e.g. when no hand is visible."""
        self._smoother.reset()

    @staticmethod
    def _raised_fingers(hand_landmarks, hand_label: str):
        """Returns which of the 5 fingers are raised, as booleans in Thumb..Pinky order."""
        lm = hand_landmarks.landmark
        thumb_raised = lm[4].x < lm[3].x if hand_label == "Right" else lm[4].x > lm[3].x
        return [
            thumb_raised,
            lm[8].y < lm[6].y,
            lm[12].y < lm[10].y,
            lm[16].y < lm[14].y,
            lm[20].y < lm[18].y,
        ]

    @staticmethod
    def _name_of_raised(raised):
        """Maps a raised-fingers list to a single finger name, or None if it's not exactly one finger."""
        if raised.count(True) != 1:
            return None
        return FINGER_NAMES[raised.index(True)]


def resize_frame(frame, scale: float = 0.75):
    width = int(frame.shape[1] * scale)
    height = int(frame.shape[0] * scale)
    return cv.resize(frame, (width, height), interpolation=cv.INTER_AREA)
