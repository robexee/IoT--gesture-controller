# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

An ESP32 (Arduino framework, PlatformIO) firmware for a WiFi/UDP-controlled buzzer and LED box — a game-show-style buzzer system. The device connects to WiFi in station mode, listens for short plain-text commands over UDP, and toggles two LEDs and a buzzer accordingly.

## Build

Standard PlatformIO workflow (VS Code PlatformIO extension or `pio` CLI):
- Build: `pio run`
- Upload to board: `pio run -t upload`
- Serial monitor: `pio device monitor` (baud 115200)

**Note:** `platformio.ini` is currently deleted from the working tree (shows as `D` in `git status`). The project will not build until it's restored. Last committed content targeted a single `esp32dev` environment (`platform = espressif32`, `board = esp32dev`, `framework = arduino`, no lib_deps).

There is no test suite — `test/` currently has no test files (PlatformIO's stock Unity boilerplate README was the only committed content, and it too is deleted from the working tree).

## Architecture

Four small wrapper/handler classes wired together in `src/main.cpp`:

- **`Led`** (`include/Led.h`, `src/Led.cpp`) — GPIO wrapper with tracked on/off state: `begin()`, `on()`, `off()`, `toggle()`, `IsOn()`.
- **`Buzzer`** (`include/Buzzer.h`, `src/Buzzer.cpp`) — GPIO wrapper: `begin()`, `on()`, `off()`, `beep(int time)` (blocking on→delay→off).
- **`WifiCommunicator`** (`include/WifiCommunicator.h`, `src/WifiCommunicator.cpp`) — station-mode WiFi + UDP listener (no AP mode, no web server). `connect()` joins WiFi and starts listening on a UDP port; `reconnect()` re-joins if the connection drops; `getCommand()` polls for an incoming UDP packet and returns it as a string (empty string if none).
- **`CommandHandler`** (`include/CommandHandler.h`, `src/CommandHandler.cpp`) — parses incoming command strings and dispatches to the `Led`/`Buzzer` objects. Recognized commands: `/buzz`, `/red_on`, `/red_off`, `/yellow_on`, `/yellow_off`. Unrecognized strings are silently ignored.

`main.cpp` owns the instances and pin assignments (red LED = GPIO23, yellow LED = GPIO22, buzzer = GPIO2), constructs `CommandHandler` with pointers to the `Led`/`Buzzer` objects, and in `loop()`: calls `wifi.reconnect()`, polls `wifi.getCommand()`, and forwards any non-empty command to `commandHandler.handleCommand(...)`.

**WiFi credentials and the UDP port are hardcoded in `main.cpp`** (SSID, password, port 4210) — treat these as secrets; avoid echoing the password back in responses, and flag before committing changes to this file that it's committing a plaintext WiFi password.

## Known issues in the current working tree

- `CommandHandler`'s constructor has a copy-paste bug: `yellow = yellowLed;` and `buzz = buzzer;` are backwards (should be `yellowLed = yellow;` and `buzzer = buzz;`). As a result the `yellowLed` and `buzzer` member pointers are never assigned from the constructor arguments, so `/yellow_on`, `/yellow_off`, and `/buzz` operate on uninitialized pointers. Not yet fixed in the working tree.
- Uncommitted fixes already in the working tree: `CommandHandler.cpp` now compares command strings with `strcmp` (the committed version used `==` on `const char*`, which never matched); `WifiCommunicator.cpp` adds a bounds check (`len < 255`) on the UDP packet read buffer.
- `platformio.ini` and `test/README` are deleted in the working tree but still tracked in git — check with the user before assuming these deletions are intentional.
