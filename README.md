# Alyn

Alyn is an embedded system for an interactive game that children with special needs play in a pool.

Illuminated LEDs are placed underwater. The player swims between them and presses each lit LED to turn it off. The game encourages movement, coordination and sensory engagement.

Technically, the project combines:
- **Embedded hardware**: an STM32 MCU (Nucleo platform) driving a display, keypad and LEDs
- **Real-time software**: Zephyr RTOS
- **Scriptable game logic**: Lua, so game modes, timing and interaction patterns can be changed without touching the firmware

---

## Demo Video

The demo shows the user interface and the gameplay flow. It starts with the **configuration screen**, where you set:
- **Game type**
- **Game duration** (or the number of LED blinks before shutdown)
- **Number of required LED presses**

<video src="https://github.com/user-attachments/assets/8c19e718-b8e6-4aec-9531-b5ccfbf1cde7" autoplay loop muted playsinline width="100%"></video>

---

## Repository layout

| Directory | Contents |
|-----------|----------|
| `game-controller/` | Zephyr firmware for the STM32 (game state, buttons, display, UART protocol, shell) |
| `common/` | Code shared by firmware and simulators: LED effects, hardware abstraction layer (`hal/`: Zephyr, Linux, Windows, emulator), Lua engine and effects, UART protocol |
| `game-pc/` | PC side: UART message handling and the Qt LED matrix simulator |
| `alyn-go/` | Terminal (TUI) LED simulator written in Go |
| `docs/` | Doxygen configuration and generated documentation |

### How it fits together
- **STM32 + Zephyr** run the game controller. Peripherals (display, keypad, LEDs) are reached through GPIO, UART, SPI and I2C.
- **Lua 5.4** is embedded in the firmware. Games and effects are Lua scripts that call a small C API for the LED matrix, timers and station LEDs.
- **A hardware abstraction layer** in `common/hal/` lets the same effects, Lua engine and game code run on the device, on Linux and on Windows. The simulators use this to run games without hardware.

---

## alyn-go: terminal LED simulator

`alyn-go/` is a terminal (TUI) replacement for the Qt LED matrix simulator in `game-pc/led_matrix_simulator`.
It is a single Go process (built on [termforge](https://github.com/yairgd/termforge)) that links the same MCU C code
from `common/` (effects, HAL, Lua 5.4.6 engine) through cgo, so games behave as on the device. It shows the LED matrix
and the 8 station LEDs, a game list and the Lua console (REPL). Click a game to start it, click a station LED to press
its button, or use `:load file.lua` / `:restart`. Press `?` for help, `Ctrl-C` to interrupt a running Lua chunk and `Ctrl-D` to quit.

Requirements: Go, CMake, a C/C++ compiler, Python 3, and [go-task](https://taskfile.dev).

```bash
cd alyn-go
task build     # CMake builds the C libs from ../common into lib/, then Go builds bin/alyn
task run       # build + start the TUI (or run ./bin/alyn directly)
task check     # build + vet + tests + gofmt
```

---

## Building the firmware

### 1. Set up Zephyr
```bash
export PROJECT=$HOME/zephyrproject          # any directory you like

python3 -m venv $PROJECT/.venv
source $PROJECT/.venv/bin/activate
pip install pyelftools west

cd $PROJECT
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v3.4.0
west update
```

### 2. Get the project
```bash
git clone git@github.com:yairgd/alyn.git
cd alyn
```

### 3. Build
```bash
export ZEPHYR_BASE=${PROJECT}/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
```

With west:
```bash
west build -b nucleo_f429zi -d nucleo_f429zi . -- -G"Unix Makefiles"   # Makefile generator
west build -b nucleo_f429zi -d nucleo_f429zi                           # Ninja (default)
```

Or directly with CMake:
```bash
mkdir nucleo_f429zi && cd nucleo_f429zi
cmake -DBOARD=nucleo_f429zi -DOVERLAY_CONFIG=prj.conf \
      -DDTC_OVERLAY_FILE=./boards/nucleo_f429zi.overlay ..   # add -GNinja for Ninja
```

To build for native Linux execution:
```bash
mkdir native_posix_64 && cd native_posix_64
cmake -DBOARD=native_posix_64 -DOVERLAY_CONFIG=prj.conf \
      -DDTC_OVERLAY_FILE=./boards/native_posix_64.overlay ..
```

---

## Flashing and debugging

Start OpenOCD (adjust `ZEPHYR_SDK` to your SDK install, e.g. `zephyr-sdk-0.16.1`):
```bash
export ZEPHYR_SDK=$HOME/zephyr-sdk-0.16.1
BOARD_DIR=$ZEPHYR_BASE/boards/arm/nucleo_f429zi/support

sudo $ZEPHYR_SDK/sysroots/x86_64-pokysdk-linux/usr/bin/openocd \
  -s $BOARD_DIR \
  -s $ZEPHYR_SDK/sysroots/x86_64-pokysdk-linux/usr/share/openocd/scripts \
  -f $BOARD_DIR/openocd.cfg -c init -c targets -c 'reset init'
```
`sudo` is only needed without a udev rule for the ST-Link.

Start GDB:
```bash
cd game-controller/nucleo_f429zi
$ZEPHYR_SDK/arm-zephyr-eabi/bin/arm-zephyr-eabi-gdb zephyr/zephyr.elf
```

Inside GDB:
```
target remote 127.0.0.1:3333
load
b game.c:169
```

---

## Supported boards

Developed on the NUCLEO-F429ZI. Other Nucleo boards supported by Zephyr should work with a matching overlay:
NUCLEO-F429ZI, NUCLEO-F439ZI, NUCLEO-F767ZI, NUCLEO-L4R5ZI, NUCLEO-H745ZI-Q.
See the [Zephyr board documentation](https://docs.zephyrproject.org/latest/boards/arm/nucleo_f429zi/doc/index.html).
