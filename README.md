# Dual-Rate LED Blinker (FreeRTOS)

[![Build & Test](https://github.com/your-username/blinky-rtos/actions/workflows/ci.yml/badge.svg)](https://github.com/your-username/blinky-rtos/actions/workflows/ci.yml)

Tiny demo that shows two independent RTOS tasks blinking LEDs at different rates on an STM32F103 ("Blue Pill").

* **Fast task**: 200 ms toggle (red LED, PC13) - 2.5 Hz blink rate
* **Slow task**: 1000 ms toggle (green LED, PB12) - 0.5 Hz blink rate

> "If you can blink two LEDs, you can build a satellite." — Every RTOS tutor ever

## Features

- ✅ **Dual-rate LED blinking** with FreeRTOS tasks
- ✅ **Complete build system** with CMake and ARM GCC
- ✅ **Automated CI/CD** with GitHub Actions
- ✅ **Unit tests** with Unity testing framework
- ✅ **Cross-platform flashing** (ST-Link, OpenOCD, J-Link)
- ✅ **Static analysis** with cppcheck
- ✅ **Documentation** with Doxygen
- ✅ **Hardware abstraction** for easy porting

## Quick Start

### Prerequisites

- **ARM GCC toolchain**: `arm-none-eabi-gcc` and friends
- **CMake** 3.20 or newer
- **Git** with submodule support
- **Flashing tool**: ST-Link, OpenOCD, or J-Link

#### Ubuntu/Debian Installation
```bash
sudo apt update
sudo apt install -y build-essential cmake git
sudo apt install -y gcc-arm-none-eabi binutils-arm-none-eabi
sudo apt install -y stlink-tools  # For ST-Link flashing
```

#### macOS Installation (with Homebrew)
```bash
brew install cmake git
brew install --cask gcc-arm-embedded
brew install stlink
```

### Build and Flash

```bash
# 1. Clone with submodules
git clone --recurse-submodules https://github.com/your-username/blinky-rtos.git
cd blinky-rtos

# 2. Add FreeRTOS submodule (if not already done)
git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git vendor/FreeRTOS-Kernel
git submodule update --init --recursive

# 3. Configure and build firmware
cmake -B build-firmware -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc.cmake
cmake --build build-firmware

# 4. Flash to Blue Pill
./flash.sh build-firmware/blinky.hex

# 5. Watch the magic! 🎉
```

### Run Tests

```bash
# Build and run unit tests
cmake -B build-tests -DBUILD_TESTING=ON
cmake --build build-tests
ctest --test-dir build-tests --verbose
```

## Hardware Setup

### STM32F103C8T6 "Blue Pill" Wiring

```
STM32F103C8T6 Blue Pill
┌─────────────────────┐
│                     │
│ PC13 ●──────────────┼── Built-in LED (Red)
│                     │
│ PB12 ●──────────────┼── External LED (Green) + 220Ω resistor to GND
│                     │
│ 3V3  ●──────────────┼── Power for external components
│ GND  ●──────────────┼── Ground
│                     │
│      [USB] ●────────┼── Programming/Power via USB
└─────────────────────┘
```

### External LED Connection
```
PB12 ──[ 220Ω ]──┤>├── GND
                 LED
```

**Note**: PC13 has a built-in LED that's active-low (on when pin is low).

## Project Structure

```
blinky-rtos/
├── .github/
│   └── workflows/ci.yml          # GitHub Actions CI/CD
├── cmake/
│   ├── arm-gcc.cmake            # ARM GCC toolchain file
│   └── STM32F103C8Tx_FLASH.ld   # Linker script for STM32F103
├── docs/
│   ├── getting_started.md       # Detailed setup guide
│   └── hardware_setup.png       # Wiring diagram
├── include/
│   └── board.h                  # Hardware abstraction layer
├── src/
│   ├── main.c                   # Application entry point
│   ├── board.c                  # Board-specific implementations
│   └── FreeRTOSConfig.h         # FreeRTOS configuration
├── tests/
│   ├── test_scheduler.c         # Unit tests
│   ├── unity_config.h           # Unity test framework config
│   └── CMakeLists.txt          # Test build configuration
├── vendor/
│   └── FreeRTOS-Kernel/        # FreeRTOS submodule
├── CMakeLists.txt              # Main build configuration
├── flash.sh                    # Multi-tool flashing script
├── Doxyfile                    # Documentation configuration
├── .gitmodules                 # Git submodule configuration
└── README.md                   # This file
```

## Development

### Adding New Features

1. **New Task**: Add task functions in `src/main.c`
2. **Hardware**: Extend `src/board.c` and `include/board.h`
3. **Tests**: Add unit tests in `tests/`
4. **Documentation**: Update README and add Doxygen comments

### Debugging

The project includes support for:
- **SWD debugging** via ST-Link
- **Semihosting** for printf output
- **Static analysis** with cppcheck
- **Memory analysis** with ARM GCC built-in tools

### Porting to Other MCUs

1. Update `cmake/STM32F103C8Tx_FLASH.ld` for your memory layout
2. Modify `src/board.c` for your GPIO configuration
3. Adjust `src/FreeRTOSConfig.h` for your system clock
4. Update `cmake/arm-gcc.cmake` for your CPU type

## CI/CD Pipeline

The GitHub Actions workflow automatically:

1. **Builds firmware** for ARM Cortex-M3
2. **Runs unit tests** on the host system
3. **Performs static analysis** with cppcheck
4. **Generates documentation** with Doxygen
5. **Creates release artifacts** for tagged versions

## Troubleshooting

### Common Issues

**Build fails with "arm-none-eabi-gcc not found"**
```bash
# Install ARM GCC toolchain
sudo apt install gcc-arm-none-eabi
# or
brew install --cask gcc-arm-embedded
```

**Flashing fails with permission errors**
```bash
# Add user to dialout group (Linux)
sudo usermod -a -G dialout $USER
# Then log out and back in
```

**FreeRTOS submodule missing**
```bash
git submodule update --init --recursive
```

### LED Not Blinking?

1. Check power: Blue Pill should have power LED on
2. Verify wiring: External LED on PB12 with proper resistor
3. Check programming: Built-in LED (PC13) should blink fast
4. Monitor with multimeter: Voltage should toggle on pins

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Make your changes and add tests
4. Ensure CI passes: Check that build and tests succeed
5. Submit a pull request

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## Resources

- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [STM32F103 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [ARM Cortex-M3 Programming](https://developer.arm.com/documentation/dui0552/a)
- [Unity Testing Framework](https://github.com/ThrowTheSwitch/Unity)

---

**Happy Blinking! 🚦**
