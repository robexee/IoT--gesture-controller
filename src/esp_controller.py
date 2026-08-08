import websocket


class EspController:
    def __init__(self, server_url: str):
        self._ws = websocket.create_connection(f"{server_url}/?role=controller")
        self._last_states = {}

    def set_red_led(self, on: bool):
        self._send_if_changed("red_led", on, "/red_on", "/red_off")

    def set_yellow_led(self, on: bool):
        self._send_if_changed("yellow_led", on, "/yellow_on", "/yellow_off")

    def set_buzzer(self, on: bool):
        self._send_if_changed("buzzer", on, "/buzz_on", "/buzz_off")

    def close(self):
        self._ws.close()

    def _send_if_changed(self, key: str, on: bool, on_command: str, off_command: str):
        if self._last_states.get(key) == on:
            return
        self._last_states[key] = on
        self._send(on_command if on else off_command)

    def _send(self, command: str):
        self._ws.send(command)
