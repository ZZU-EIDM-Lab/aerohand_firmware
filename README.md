# Aero Hand Open firmware (PlatformIO)

This project migrates the firmware sources from `../aero-hand-open/firmware/main/`
(upstream commit `7bb2f23485a7d440960c21046da642de132b7745`) into a standalone
PlatformIO project for the Seeed Studio XIAO ESP32S3. The original source is
preserved as a reference in `../aero-hand-open/`.

## Build

Open this directory in VS Code with the PlatformIO extension. The default
environment is `right_hand`; use `left_hand` for a left-hand device. The
Feetech `FTServo_Arduino` library is installed from the pinned GitHub tag in
`platformio.ini`.

From a terminal with PlatformIO available:

```sh
pio run -e right_hand
pio run -e left_hand
```

## Upload and monitor

Verify the hand type before uploading. On power-on or brownout, the firmware
automatically starts homing, which moves all seven servos. Only upload with
the matching hand connected and ready for motion.

```sh
pio run -e right_hand -t upload
pio device monitor -b 921600
```

For a left-hand device, replace `right_hand` with `left_hand` in the upload
command. The USB host serial rate is 921600 baud; the servo bus on `Serial2`
uses 1 Mbps with RX pin 2 and TX pin 3.

The upstream firmware guide is in `../aero-hand-open/firmware/README.md`.
Its historical prebuilt binaries and screenshots are not part of this source
project. The upstream firmware is licensed under Apache-2.0; see `LICENSE`.
