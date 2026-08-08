const { WebSocketServer } = require("ws");
const { parse } = require("url");

const PORT = process.env.PORT || 8080;
const wss = new WebSocketServer({ port: PORT });

let esp32Socket = null;
const controllerSockets = new Set();

wss.on("connection", (socket, req) => {
    const { query } = parse(req.url, true);
    const role = query.role;

    if (role === "esp32") {
        esp32Socket = socket;
        console.log("ESP32 connected");
        socket.on("close", () => {
            if (esp32Socket === socket) esp32Socket = null;
            console.log("ESP32 disconnected");
        });
    } else if (role === "controller") {
        controllerSockets.add(socket);
        console.log("Controller connected");
        socket.on("close", () => controllerSockets.delete(socket));
    } else {
        socket.close(1008, "role query param required (esp32|controller)");
        return;
    }

    socket.on("message", (data) => {
        if (role !== "controller") return;

        const command = data.toString();
        if (esp32Socket) {
            esp32Socket.send(command);
        } else {
            console.warn("No ESP32 connected, dropping command:", command);
        }
    });
});

console.log(`Relay server listening on ws://0.0.0.0:${PORT}`);
