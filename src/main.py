"""
main.py

Entry point for the desktop side of the project: opens the default
webcam, drives FingerIdentifier (camera.py) on each frame, and shows
the result in a window. Raise a single finger at a time to see it
identified.
"""

import time

import cv2 as cv

from camera import FingerIdentifier, resize_frame

FONT = cv.FONT_HERSHEY_COMPLEX
WINDOW_NAME = "Finger Identifier"


def preprocess_frame(frame):
    """Mirrors the frame for a natural view and scales it up for display."""
    frame = cv.flip(frame, 1)
    return resize_frame(frame, 2)


def compute_fps(prev_frame_time):
    """Returns (fps, now) given the timestamp of the previous frame."""
    now = time.time()
    fps = 1 / (now - prev_frame_time) if prev_frame_time else 0
    return fps, now


def draw_fps(frame, fps):
    cv.putText(frame, f"FPS: {int(fps)}", (frame.shape[1] - 150, 40), FONT, 1, (255, 255, 255), 1)


def draw_finger_label(frame, finger):
    cv.putText(frame, f"Finger: {finger or '-'}", (10, 40), FONT, 1, (255, 255, 255), 1)


def detect_finger(identifier, frame):
    """Runs finger detection on a frame, drawing hand landmarks in place.

    Returns the identified finger name, or None if no hand is visible.
    """
    rgb_frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
    results = identifier.process(rgb_frame)

    if not results.multi_hand_landmarks:
        identifier.reset()
        return None

    hand_landmarks = results.multi_hand_landmarks[0]
    hand_label = results.multi_handedness[0].classification[0].label

    identifier.draw_landmarks(frame, hand_landmarks)
    return identifier.identify_finger(hand_landmarks, hand_label)


def main():
    video = cv.VideoCapture(0)
    identifier = FingerIdentifier()
    prev_frame_time = 0.0

    while True:
        has_frame, frame = video.read()
        if not has_frame:
            break

        frame = preprocess_frame(frame)
        fps, prev_frame_time = compute_fps(prev_frame_time)
        draw_fps(frame, fps)

        finger = detect_finger(identifier, frame)
        draw_finger_label(frame, finger)

        cv.imshow(WINDOW_NAME, frame)
        if cv.waitKey(10) & 0xFF == ord("q"):
            break

    video.release()
    cv.destroyAllWindows()


if __name__ == "__main__":
    main()
