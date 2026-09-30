# cf-sim – host clockface preview

Renders any `Clockface` on Linux to 64x64 BMP files, no hardware needed.

## Build

```bash
make -C tools/cf-sim [FACE_DIR=<face-dir>]
# or: cmake -S tools/cf-sim -B tools/cf-sim/build [-DFACE_DIR=<face-dir>]
```

`FACE_DIR` defaults to the house face. All `.cpp` files under it are
built automatically; objects land in `obj/` so face repos stay clean.

## Run

```bash
./tools/cf-sim/sim --out /tmp/frames            # day sweep
./tools/cf-sim/sim --time 7:30 --out /tmp/frames
./tools/cf-sim/sim --time 19:05 --out /tmp/frames --frames 120
```

## Tests

```bash
make -C tools/cf-sim run run_text
# or: ctest --test-dir tools/cf-sim/build
```

## Internals

Host `SimRenderer` (framebuffer) + `SimClock` (`millis()`) +
`FakeDateTime` (deterministic clock) + vendored Adafruit GFX core.
Links the real `cw-gfx-engine` (`Locator`/`EventBus`/`Sprite`);
`Arduino.h`/`M5Stack.h`/`HostEngine.cpp` shim the remaining
device-only APIs.
