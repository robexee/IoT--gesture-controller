import time
import cv2 as cv

from camera import FINGER_NAMES, FingerIdentifier, resize_frame
from esp_controller import EspController

FONT = cv.FONT_HERSHEY_COMPLEX
WINDOW_NAME = "Finger Identifier"

"""ADRESA SERVERULUI NODE.JS DE RELAY (nu a ESP32-ului)"""
SERVER_URL = "ws://192.168.1.37:8080"

NO_FINGERS_RAISED = {name: False for name in FINGER_NAMES}


def preprocess_frame(frame):
    frame = cv.flip(frame, 1)
    return resize_frame(frame, 2)


def compute_fps(prev_frame_time):
    now = time.time()
    fps = 1 / (now - prev_frame_time) if prev_frame_time else 0
    return fps, now


def draw_fps(frame, fps):
    cv.putText(frame, f"FPS: {int(fps)}", (frame.shape[1] - 150, 40), FONT, 1, (255, 255, 255), 1)


def draw_raised_fingers(frame, raised_fingers):
    raised_names = [name for name, is_up in raised_fingers.items() if is_up]
    cv.putText(frame, f"Raised: {', '.join(raised_names) or '-'}", (10, 40), FONT, 1, (255, 255, 255), 1)


def detect_raised_fingers(identifier, frame):
    rgb_frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
    results = identifier.process(rgb_frame)

    if not results.multi_hand_landmarks:
        identifier.reset()
        return NO_FINGERS_RAISED

    hand_landmarks = results.multi_hand_landmarks[0]
    hand_label = results.multi_handedness[0].classification[0].label

    identifier.draw_landmarks(frame, hand_landmarks)
    return identifier.get_raised_fingers(hand_landmarks, hand_label)


def apply_finger_controls(controller, raised_fingers, red_led_on):
    controller.set_red_led(red_led_on)
    controller.set_buzzer(raised_fingers["Thumb"])
    controller.set_yellow_led(raised_fingers["Middle"])


def toggle_on_rising_edge(index_raised, prev_index_raised, red_led_on):
    if index_raised and not prev_index_raised:
        return not red_led_on
    return red_led_on


def main():
    video = cv.VideoCapture(0)
    identifier = FingerIdentifier()
    controller = EspController(SERVER_URL)
    prev_frame_time = 0.0
    prev_index_raised = False
    red_led_on = False

    while True:
        has_frame, frame = video.read()
        if not has_frame:
            break

        frame = preprocess_frame(frame)
        fps, prev_frame_time = compute_fps(prev_frame_time)
        draw_fps(frame, fps)

        raised_fingers = detect_raised_fingers(identifier, frame)
        red_led_on = toggle_on_rising_edge(raised_fingers["Pinky"], prev_index_raised, red_led_on)
        prev_index_raised = raised_fingers["Pinky"]

        apply_finger_controls(controller, raised_fingers, red_led_on)
        draw_raised_fingers(frame, raised_fingers)

        cv.imshow(WINDOW_NAME, frame)
        if cv.waitKey(10) & 0xFF == ord("q"):
            break

    video.release()
    cv.destroyAllWindows()
    controller.close()


if __name__ == "__main__":
    main()
