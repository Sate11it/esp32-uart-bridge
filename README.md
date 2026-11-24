# esp32-uart-bridge

Transparent, low-latency USB<->UART bridge, using an ESP32 as a pass-through.

## Why?

- You have an ESP32 dev board lying around and no USB-UART adapter.
- You want a minimal, readable ESP-IDF UART example.

The ESP32 does **not** speak the PN532 protocol. It forwards raw bytes
between two UARTs as fast as it can. All protocol handling (framing,
checksums, ACKs) stays on the host side, so you can keep using `libnfc`,
`pn532.py`, or any tool that talks to a serial-connected PN532.

The bridge is device-agnostic; the reference example below uses a PN532 NFC module

## Wiring

| ESP32            | PN532 module |
|------------------|--------------|
| GPIO17 (UART2 TX)| RX           |
| GPIO16 (UART2 RX)| TX           |
| 3V3              | VCC          |
| GND              | GND          |

Host side: ESP32 USB serial (UART0), 115200 8N1.
Set the PN532 module to UART mode (jumpers/DIP per your module manual).

## Build & flash

Built with the Arduino core for ESP32 (Arduino IDE or PlatformIO).

## Usage

Point your host tool at the ESP32 COM port, 115200 8N1.
Example with libnfc:

    nfc-list   # with conn string: pn532_uart:/dev/ttyUSB0:115200

## Performance

Throughput is bounded by 115200 baud on both links; forwarding adds
no framing overhead.

## Known limitations (read this)

- **3.3V ONLY!!!**
- **No error checks**
- **Busy-wait polling.** `loop()` spins at 240 MHz with zero-timeout
  reads. Fine on USB power; do not expect battery life or light-sleep.
- **Do not print debug output.** The console lives on UART0; any
  `Serial.print` / `ESP_LOGx` injects garbage into the PN532 protocol
  stream and corrupts frames.
- **No PN532 wake/sleep management.** The host tool must handle wake-up
  sequences; the bridge is dumb by design.

## Roadmap

- [ ] `esp_err_t` checks and graceful failure
- [ ] Latency benchmark script
- [ ] PN532 wake/sleep passthrough helpers

## License

MIT
