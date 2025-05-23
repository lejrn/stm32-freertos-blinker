<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

# STM32 FreeRTOS Dual-Rate LED Blinker Project

## Project Overview
This is an embedded systems project targeting the STM32F103C8T6 "Blue Pill" microcontroller. The project demonstrates FreeRTOS multitasking by running two independent LED blinking tasks at different rates.

## Key Technologies
- **Target MCU**: STM32F103C8T6 (ARM Cortex-M3, 72MHz)
- **RTOS**: FreeRTOS (real-time operating system)
- **Build System**: CMake with ARM GCC toolchain
- **Testing**: Unity testing framework for unit tests
- **Documentation**: Doxygen for API documentation

## Code Style Guidelines
- Use **C11 standard** for all source code
- Follow **embedded C best practices**:
  - Minimize dynamic memory allocation
  - Use `volatile` for hardware registers
  - Prefer fixed-width integers (`uint32_t`, `uint8_t`)
  - Use `const` for read-only data
- **Function naming**: Use `snake_case` (e.g., `board_init`, `blink_fast_task`)
- **Constants**: Use `UPPER_CASE` for #define macros
- **Comments**: Use Doxygen-style comments for functions:
  ```c
  /**
   * @brief Brief description of function
   * @param param1 Description of parameter
   * @return Description of return value
   */
  ```

## Hardware Abstraction
- All hardware-specific code should be in `src/board.c` and `include/board.h`
- Use the board abstraction layer for GPIO operations
- Pin definitions should be in `include/board.h`
- Keep main application logic hardware-independent

## FreeRTOS Conventions
- Task functions should have `void*` parameter and infinite loop
- Use `vTaskDelay(pdMS_TO_TICKS(ms))` for delays
- Task priorities: Higher numbers = higher priority
- Stack sizes: Use `configMINIMAL_STACK_SIZE` as baseline
- Always check return values from FreeRTOS API calls

## Memory Management
- This project uses **heap_4** FreeRTOS memory scheme
- Avoid `malloc()`/`free()` - use FreeRTOS memory management
- Total heap size is configured in `FreeRTOSConfig.h`
- Prefer static allocation where possible

## Testing Strategy
- **Unit tests** in `tests/` directory using Unity framework
- Tests should compile for host system (x86/ARM64) not target MCU
- Mock hardware dependencies in tests
- Use CTest for test execution

## Build System Notes
- **Cross-compilation**: Uses `cmake/arm-gcc.cmake` toolchain
- **Host compilation**: Standard CMake for tests
- Output files: `.hex` for flashing, `.elf` for debugging
- Use `BUILD_TESTING=ON` to enable unit tests

## Debugging Support
- Project supports SWD debugging with ST-Link
- Use `.elf` file for debugging (contains debug symbols)
- Semihosting can be enabled for printf output
- Hardware breakpoints available on Cortex-M3

## Common Patterns
When suggesting code improvements or new features:

1. **New GPIO pins**: Add to `include/board.h` and implement in `src/board.c`
2. **New tasks**: Follow existing pattern in `src/main.c`
3. **Hardware drivers**: Create separate `.c/.h` files in appropriate directories
4. **Configuration**: Add to `src/FreeRTOSConfig.h` for RTOS settings
5. **Tests**: Create corresponding test file in `tests/` directory

## Performance Considerations
- Cortex-M3 has **no FPU** - avoid floating-point math in ISRs
- Flash: 64KB, RAM: 20KB - be mindful of memory usage
- System clock: 72MHz - optimize for power and performance
- Interrupt latency: Keep ISRs short and fast

## Porting Guidelines
To port this project to other STM32 families:
1. Update linker script in `cmake/`
2. Modify `src/board.c` for different GPIO registers
3. Adjust clock configuration in `SystemClock_Config()`
4. Update `cmake/arm-gcc.cmake` for different CPU core
5. Verify FreeRTOS port compatibility

Remember: This is a learning project emphasizing clean, portable embedded code with proper RTOS usage patterns.
