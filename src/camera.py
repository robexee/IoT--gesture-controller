from collections import deque, Counter

import cv2 as cv
import mediapipe as mp

FINGER_NAMES = ("Thumb", "Index", "Middle", "Ring", "Pinky")


class MajorityVoteSmoother:

    def __init__(self, window_size: int = 5):
        self._buffer = deque(maxlen=window_size)

    def push(self, value):
        self._buffer.append(value)
        return Counter(self._buffer).most_common(1)[0][0]

    def reset(self):
        self._buffer.clear()


class FingerIdentifier:

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
        return self._hands.process(rgb_frame)

    def draw_landmarks(self, frame, hand_landmarks):
        self._mp_drawing.draw_landmarks(
            frame, hand_landmarks, self._mp_hands.HAND_CONNECTIONS, self._landmark_style
        )

    def get_raised_fingers(self, hand_landmarks, hand_label: str):
        raised = self._raised_fingers(hand_landmarks, hand_label)
        smoothed = self._smoother.push(tuple(raised))
        return dict(zip(FINGER_NAMES, smoothed))

    def reset(self):
        self._smoother.reset()

    @staticmethod
    def _raised_fingers(hand_landmarks, hand_label: str):
        lm = hand_landmarks.landmark
        thumb_raised = lm[4].x < lm[3].x if hand_label == "Right" else lm[4].x > lm[3].x
        return [
            thumb_raised,
            lm[8].y < lm[6].y,
            lm[12].y < lm[10].y,
            lm[16].y < lm[14].y,
            lm[20].y < lm[18].y,
        ]


def resize_frame(frame, scale: float = 0.75):
    width = int(frame.shape[1] * scale)
    height = int(frame.shape[0] * scale)
    return cv.resize(frame, (width, height), interpolation=cv.INTER_AREA)
