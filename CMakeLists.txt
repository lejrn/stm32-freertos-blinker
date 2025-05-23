cmake_minimum_required(VERSION 3.20)

project(blinky VERSION 1.0.0 LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set Cortex-M3 compiler flags only when cross-compiling
if(CMAKE_CROSSCOMPILING AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -mthumb -Wall -Wextra -Os -ffunction-sections -fdata-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mcpu=cortex-m3 -mthumb -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections -T${CMAKE_CURRENT_SOURCE_DIR}/cmake/STM32F103C8Tx_FLASH.ld")
endif()

# FreeRTOS configuration (only for cross-compilation)
if(CMAKE_CROSSCOMPILING)
    set(FREERTOS_HEAP "4")
    set(FREERTOS_PORT "GCC_ARM_CM3")

    # Create freertos_config target required by FreeRTOS-Kernel
    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config SYSTEM
        INTERFACE
            src  # Directory containing FreeRTOSConfig.h
            include
    )
    target_compile_definitions(freertos_config
        INTERFACE
            projCOVERAGE_TEST=0
    )

    # Add FreeRTOS-Kernel
    add_subdirectory(vendor/FreeRTOS-Kernel)
endif()

# Include directories
include_directories(
    include
    src
)

if(CMAKE_CROSSCOMPILING)
    include_directories(
        vendor/FreeRTOS-Kernel/include
        vendor/FreeRTOS-Kernel/portable/GCC/ARM_CM3
    )
endif()

# Source files (only for cross-compilation)
if(CMAKE_CROSSCOMPILING)
    set(SOURCES
        src/main.c
        src/board.c
    )

    # Create executable
    add_executable(${PROJECT_NAME} ${SOURCES})

    # Link libraries
    target_link_libraries(${PROJECT_NAME} 
        freertos_kernel
        freertos_kernel_port
    )

    # Generate hex file
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
        COMMENT "Generating HEX file"
    )
endif()

# Enable testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
