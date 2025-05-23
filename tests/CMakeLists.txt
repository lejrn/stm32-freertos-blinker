# Test configuration for host-side unit tests
cmake_minimum_required(VERSION 3.20)

# Download Unity testing framework
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)

# Create test executable
add_executable(test_scheduler
    test_scheduler.c
    unity_config.h
)

# Link with Unity
target_link_libraries(test_scheduler unity)

# Include test configuration
target_include_directories(test_scheduler PRIVATE
    .
    ../include
)

# Add test to CTest
add_test(NAME scheduler_tests COMMAND test_scheduler)

# Compiler options for tests (host compilation)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(test_scheduler PRIVATE
        -Wall
        -Wextra
        -std=c11
    )
endif()
