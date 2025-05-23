# Getting Started with Dual-Rate LED Blinker

This guide will walk you through setting up and running the dual-rate LED blinker project on your STM32F103 "Blue Pill" development board.

## Prerequisites

### Hardware Required

- **STM32F103C8T6 "Blue Pill"** development board
- **ST-Link V2** programmer (or compatible)
- **External LED** (any color, we'll use green in this example)
- **220Ω resistor** (for current limiting)
- **Breadboard** and **jumper wires**
- **USB cable** (for powering the Blue Pill)

### Software Required

#### Windows (with WSL2 recommended)
```bash
# In WSL2 Ubuntu:
sudo apt update
sudo apt install -y build-essential cmake git
sudo apt install -y gcc-arm-none-eabi binutils-arm-none-eabi
sudo apt install -y stlink-tools
```

#### macOS
```bash
# Using Homebrew:
brew install cmake git
brew install --cask gcc-arm-embedded
brew install stlink
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y build-essential cmake git
sudo apt install -y gcc-arm-none-eabi binutils-arm-none-eabi
sudo apt install -y stlink-tools

# Add user to dialout group for ST-Link access:
sudo usermod -a -G dialout $USER
# Log out and back in for this to take effect
```

## Hardware Setup

### 1. Blue Pill Overview

The STM32F103C8T6 "Blue Pill" is a popular, low-cost development board featuring:
- 32-bit ARM Cortex-M3 core @ 72MHz
- 64KB Flash memory
- 20KB SRAM
- Built-in LED on PC13 (active low)
- USB connector for power and programming

### 2. LED Connections

```
Blue Pill Pinout (relevant pins):
    ┌─────────────────┐
    │     USB         │
    │   ┌─────────┐   │
    │   │         │   │
GND │ ● │         │   │
3V3 │ ● │         │   │
    │   │         │   │
    │   │   STM   │   │
    │   │  32F103 │   │
    │   │         │   │
B12 │ ● │         │   │ ← External Green LED
    │   │         │   │
C13 │ ● │         │   │ ← Built-in Red LED
    │   └─────────┘   │
    └─────────────────┘
```

### 3. External LED Wiring

Connect the external green LED to pin PB12:

```
PB12 ───┬──[ 220Ω ]──┤>├── GND
        │            LED
        │
      ┌─┴─┐
      │ + │  ← LED positive (longer leg)
      │ - │  ← LED negative (shorter leg)
      └───┘
```

**Important Notes:**
- PC13 (built-in LED) is **active low** - it turns ON when the pin is LOW
- PB12 (external LED) is **active high** - it turns ON when the pin is HIGH
- Always use a current-limiting resistor (220Ω is safe for most LEDs)

### 4. ST-Link Connection

Connect ST-Link V2 to the Blue Pill SWD pins:

```
ST-Link V2    Blue Pill
---------     ---------
3V3       →   3V3
GND       →   GND
SWDIO     →   DIO (PA13)
SWCLK     →   CLK (PA14)
```

## Project Setup

### 1. Clone the Repository

```bash
git clone --recurse-submodules https://github.com/your-username/blinky-rtos.git
cd blinky-rtos
```

If you forgot `--recurse-submodules`, add the FreeRTOS submodule:
```bash
git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git vendor/FreeRTOS-Kernel
git submodule update --init --recursive
```

### 2. Verify Toolchain Installation

```bash
# Check ARM GCC installation:
arm-none-eabi-gcc --version
# Should output something like: arm-none-eabi-gcc (GNU Arm Embedded Toolchain 10.3-2021.10)

# Check CMake:
cmake --version
# Should be 3.20 or newer

# Check ST-Link tools:
st-info --version
# Should show ST-Link version
```

### 3. Build the Firmware

```bash
# Configure the build system:
cmake -B build-firmware -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc.cmake

# Build the firmware:
cmake --build build-firmware

# Check for successful build:
ls build-firmware/blinky.hex
# Should show the hex file
```

### 4. Flash the Firmware

```bash
# Make sure Blue Pill is connected via ST-Link:
st-info --probe
# Should detect the STM32F103 device

# Flash the firmware:
./flash.sh build-firmware/blinky.hex

# Or manually with st-flash:
st-flash write build-firmware/blinky.hex 0x8000000
```

## Expected Behavior

After successful flashing, you should observe:

1. **Red LED (PC13 - built-in)**: Fast blinking at 2.5 Hz
   - ON for 200ms, OFF for 200ms
   - This is controlled by the high-priority fast task

2. **Green LED (PB12 - external)**: Slow blinking at 0.5 Hz
   - ON for 1000ms, OFF for 1000ms
   - This is controlled by the low-priority slow task

3. **Independent Operation**: Both LEDs blink independently, demonstrating FreeRTOS multitasking

## Troubleshooting

### Build Issues

**Problem**: `arm-none-eabi-gcc: command not found`
```bash
# Solution: Install ARM GCC toolchain
sudo apt install gcc-arm-none-eabi  # Linux
brew install --cask gcc-arm-embedded  # macOS
```

**Problem**: `CMake version too old`
```bash
# Solution: Install newer CMake
pip3 install cmake  # Alternative install method
# or update your package manager
```

**Problem**: `FreeRTOS files not found`
```bash
# Solution: Initialize submodules
git submodule update --init --recursive
```

### Flashing Issues

**Problem**: `ST-Link not detected`
```bash
# Check connections and try:
st-info --probe

# Check permissions (Linux):
sudo usermod -a -G dialout $USER
# Then log out and back in
```

**Problem**: `Permission denied` when flashing
```bash
# Try with sudo (not recommended for regular use):
sudo ./flash.sh build-firmware/blinky.hex

# Better solution: Fix udev rules
echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE="0666"' | sudo tee /etc/udev/rules.d/99-stlink.rules
sudo udevadm control --reload-rules
```

### Runtime Issues

**Problem**: No LEDs blinking
1. Check power: Blue Pill power LED should be on
2. Verify ST-Link connection and try reflashing
3. Check external LED wiring and polarity
4. Measure voltage on PB12 with multimeter (should toggle 0V ↔ 3.3V)

**Problem**: Only built-in LED blinking
1. Check external LED connection to PB12
2. Verify LED polarity (longer leg to positive)
3. Test LED with direct 3.3V connection

**Problem**: LEDs blinking at wrong rate
1. This usually indicates successful operation! The built-in LED is active-low
2. Fast task: PC13 toggles every 200ms (LED appears to blink fast)
3. Slow task: PB12 toggles every 1000ms (LED blinks slow)

## Next Steps

Once you have the basic blinker working:

1. **Modify timing**: Change the delays in `src/main.c`
2. **Add more LEDs**: Connect additional LEDs to other GPIO pins
3. **Add button input**: Use interrupts to control blinking
4. **Add UART logging**: Send debug messages over serial
5. **Port to other MCUs**: Adapt for STM32F4, STM32L4, etc.

## Development Environment

### VS Code Setup (Recommended)

1. Install extensions:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)
   - Cortex-Debug (marus25)

2. Open the project folder in VS Code

3. Use Ctrl+Shift+P and run "CMake: Configure" to set up the build

4. Use the integrated terminal for building and flashing

### Debugging with SWD

The project supports SWD debugging. Add this to your `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug STM32",
            "type": "cortex-debug",
            "request": "launch",
            "executable": "${workspaceFolder}/build-firmware/blinky.elf",
            "device": "STM32F103C8",
            "servertype": "stlink"
        }
    ]
}
```

## Understanding the Code

### FreeRTOS Tasks

The application creates two independent tasks:

```c
// High priority task (2) - runs more frequently
xTaskCreate(blink_fast_task, "FastBlink", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

// Low priority task (1) - runs when high priority task is sleeping
xTaskCreate(blink_slow_task, "SlowBlink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
```

### Task Scheduling

FreeRTOS uses preemptive scheduling:
1. Fast task runs, toggles LED, then sleeps for 200ms
2. While fast task sleeps, slow task runs, toggles LED, then sleeps for 1000ms
3. When fast task wakes up, it preempts the slow task (if still running)
4. This creates independent, concurrent LED blinking

### Hardware Abstraction

The `board.h` and `board.c` files provide a hardware abstraction layer:
- `board_init()`: Configures clocks and GPIO
- `board_toggle()`: Toggles a GPIO pin
- Pin definitions: `LED_RED` and `LED_GREEN`

This makes the code portable to other STM32 variants by only changing the board files.

Happy blinking! 🚦
