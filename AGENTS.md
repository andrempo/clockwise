# Repository Guidelines

## Project Structure & Module Organization

Clockwise is firmware for an ESP32-powered LED wall clock. The primary
PlatformIO project is in `firmware/`: application code is in `firmware/src/`,
shared libraries are in `firmware/lib/`, and each clockface lives in
`firmware/clockfaces/cw-cf-0xNN/`. Clockface directories contain their
`Clockface.cpp`, `Clockface.h`, assets, and a `CMakeLists.txt`. PlatformIO tests
are under `firmware/test/test_native/` and `firmware/test/test_embedded/`.
The root `main/` and `CMakeLists.txt` support ESP-IDF builds. ESPHome support
is an external component in `firmware/esphome/components/clockwise/`.

## Build, Test, and Development Commands

Run commands from the repository root unless noted otherwise:

- `pio run -d firmware` builds the default `esp32dev` firmware environment.
- `pio run -d firmware -t upload` builds and flashes a connected board.
- `pio device monitor -d firmware` opens the serial monitor at 115200 baud.
- `pio test -d firmware -e native` runs host-side Unity tests; this skips
  embedded-only tests.
- `idf.py build` builds the ESP-IDF project after the ESP-IDF environment is
  initialized.
- `cd firmware/esphome && uv run esphome clockwise.yaml` runs the ESPHome
  configuration using the dependencies declared in `firmware/pyproject.toml`.

Dependencies are declared in `firmware/platformio.ini`; do not vendor build
outputs such as `.pio/`.

## Coding Style & Naming Conventions

Use the surrounding C++ style: two-space indentation, braces on the same line
as declarations, and concise Arduino-compatible code. Use `PascalCase` for
classes (`CWDateTime`), `camelCase` for methods and variables, and `UPPER_SNAKE_CASE`
for macros/constants. Keep clockface IDs and directories in the established
`cw-cf-0xNN` format. There is no committed formatter or linter; avoid unrelated
formatting changes and match the file you edit.

## Testing Guidelines

Add focused Unity tests when logic can run on the native target. Name test
functions descriptively with the existing `test_<behavior>` convention and keep
them in the matching `firmware/test/` target directory. Run the native test
command before submitting; validate hardware-dependent changes on an ESP32 when
available.

## Commit & Pull Request Guidelines

Recent history favors short, imperative subjects, often using Conventional
Commit prefixes such as `feat:` and `refactor:`; use `Fix ...` for focused bug
fixes. Keep each commit scoped to one change. Pull requests should explain the
user-visible or hardware impact, link related issues when applicable, list the
commands run, and include screenshots or serial output for UI, display, or
configuration changes.
