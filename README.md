# Buzzer LoL

ESP32 firmware (Arduino framework, PlatformIO) for a WiFi/UDP-controlled buzzer and LED box — a game-show-style buzzer system. The device joins WiFi in station mode, listens for short plain-text commands over UDP, and toggles two LEDs and a buzzer accordingly. A separate desktop app can drive it hands-free using webcam-based finger tracking.

## How it works

```
Desktop (main.py) ──UDP──▶ ESP32 (main.cpp)
  camera.py: hand/finger        WifiCommunicator: WiFi + UDP listener
  detection via MediaPipe       CommandHandler: parses commands
  esp_controller.py: sends      Led / Buzzer: GPIO output
  state-change commands
```

- **Index finger up** → red LED
- **Middle finger up** → yellow LED
- **Thumb up** → buzzer

## Firmware (`src/main.cpp`, `include/`, `lib/`)

| Class | Responsibility |
|---|---|
| `Led` | GPIO wrapper with tracked on/off state (`begin`, `on`, `off`, `toggle`, `IsOn`) |
| `Buzzer` | GPIO wrapper (`begin`, `setActive(bool)`) — buzzer stays on continuously while active |
| `WifiCommunicator` | Station-mode WiFi + UDP listener. `connect()` is idempotent (safe to call every loop). `getCommand()` polls for an incoming UDP packet |
| `CommandHandler` | Parses incoming strings and dispatches to `Led`/`Buzzer` |

Pin assignments: red LED = GPIO23, yellow LED = GPIO22, buzzer = GPIO2. UDP port: 4210.

### Recognized commands

```
/buzz_on   /buzz_off
/red_on    /red_off
/yellow_on /yellow_off
```

Unrecognized strings are silently ignored.

### Build

Standard PlatformIO workflow:

```
pio run              # build
pio run -t upload    # flash to board
pio device monitor    # serial monitor, 115200 baud
```

WiFi SSID/password and the UDP port are hardcoded in `src/main.cpp` — set your own credentials before flashing. **Do not commit real WiFi credentials to this file.**

## Desktop side (`src/camera.py`, `src/esp_controller.py`, `src/main.py`)

A separate Python process that drives the ESP32 over the same UDP protocol `CommandHandler` understands.

- `camera.py` — `FingerIdentifier` (MediaPipe) reports which of the 5 fingers are raised per frame, smoothed via `MajorityVoteSmoother` (majority vote over a rolling window) to avoid flicker.
- `esp_controller.py` — `EspController` wraps a UDP socket; exposes `set_red_led`, `set_yellow_led`, `set_buzzer` (bools), and only sends a packet when a given output's state actually changes.
- `main.py` — entry point: opens the webcam, runs `FingerIdentifier` per frame, maps fingers to outputs via `EspController`.

Before running, set `ESP32_IP` at the top of `main.py` to the device's actual IP (printed to Serial once `wifi.connect()` succeeds on the firmware side).

```
pip install opencv-python mediapipe
python src/main.py
```

## Known issues / TODO

- FreeRTOS is not used explicitly — `loop()` is a single sequential task (WiFi read + command handling on one thread). Splitting UDP listening and command dispatch into separate FreeRTOS tasks connected by a queue would remove the polling loop and make room for additional concurrent inputs (e.g. a physical button) without blocking UDP reception.
- No automated test suite yet (`test/` has no test files).
