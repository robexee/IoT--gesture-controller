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


def main():
    video = cv.VideoCapture(0)
    identifier = FingerIdentifier()
    prev_frame_time = 0.0

    while True:
        has_frame, frame = video.read()
        if not has_frame:
            break

        frame = cv.flip(frame, 1)
        frame = resize_frame(frame, 2)
        rgb_frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)

        now = time.time()
        fps = 1 / (now - prev_frame_time) if prev_frame_time else 0
        prev_frame_time = now
        cv.putText(frame, f"FPS: {int(fps)}", (frame.shape[1] - 150, 40), FONT, 1, (255, 255, 255), 1)

        results = identifier.process(rgb_frame)

        if results.multi_hand_landmarks:
            hand_landmarks = results.multi_hand_landmarks[0]
            hand_label = results.multi_handedness[0].classification[0].label

            finger = identifier.identify_finger(hand_landmarks, hand_label)
            identifier.draw_landmarks(frame, hand_landmarks)

            cv.putText(frame, f"Finger: {finger or '-'}", (10, 40), FONT, 1, (255, 255, 255), 1)
        else:
            identifier.reset()

        cv.imshow("Finger Identifier", frame)

        if cv.waitKey(10) & 0xFF == ord("q"):
            break

    video.release()
    cv.destroyAllWindows()


if __name__ == "__main__":
    main()
