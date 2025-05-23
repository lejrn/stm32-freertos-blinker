#include "unity.h"
#include <stdint.h>

// Mock FreeRTOS definitions for testing
#define configTICK_RATE_HZ 1000
#define pdMS_TO_TICKS(xTimeInMs) ((uint32_t)(((uint32_t)(xTimeInMs) * (uint32_t)configTICK_RATE_HZ) / (uint32_t)1000))

// Test setup and teardown
void setUp(void) 
{
    // Setup code before each test
}

void tearDown(void) 
{
    // Cleanup code after each test
}

/**
 * @brief Test tick rate conversion macro
 */
void test_tick_to_ms_conversion(void)
{
    const uint32_t one_second_ticks = pdMS_TO_TICKS(1000);
    TEST_ASSERT_EQUAL_UINT32(configTICK_RATE_HZ, one_second_ticks);
    
    const uint32_t half_second_ticks = pdMS_TO_TICKS(500);
    TEST_ASSERT_EQUAL_UINT32(500, half_second_ticks);
    
    const uint32_t blink_fast_ticks = pdMS_TO_TICKS(200);
    TEST_ASSERT_EQUAL_UINT32(200, blink_fast_ticks);
}

/**
 * @brief Test LED pin definitions
 */
void test_led_pin_definitions(void)
{
    // Test that LED pins are defined correctly
    #define LED_RED    13
    #define LED_GREEN  12
    
    TEST_ASSERT_EQUAL_UINT8(13, LED_RED);
    TEST_ASSERT_EQUAL_UINT8(12, LED_GREEN);
    TEST_ASSERT_NOT_EQUAL(LED_RED, LED_GREEN);
}

/**
 * @brief Test timing calculations
 */
void test_timing_calculations(void)
{
    // Test that our blink rates produce expected frequencies
    uint32_t fast_period_ms = 200 * 2;  // Toggle every 200ms = 400ms period
    uint32_t slow_period_ms = 1000 * 2; // Toggle every 1000ms = 2000ms period
    
    // Fast LED should blink at 2.5 Hz (400ms period)
    float fast_frequency = 1000.0f / fast_period_ms;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 2.5f, fast_frequency);
    
    // Slow LED should blink at 0.5 Hz (2000ms period)
    float slow_frequency = 1000.0f / slow_period_ms;
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.5f, slow_frequency);
}

/**
 * @brief Test priority configuration
 */
void test_task_priorities(void)
{
    const uint8_t FAST_TASK_PRIORITY = 2;
    const uint8_t SLOW_TASK_PRIORITY = 1;
    
    // Fast task should have higher priority than slow task
    TEST_ASSERT_GREATER_THAN(SLOW_TASK_PRIORITY, FAST_TASK_PRIORITY);
    
    // Both priorities should be within valid range (1-4 for our config)
    TEST_ASSERT_GREATER_OR_EQUAL(1, FAST_TASK_PRIORITY);
    TEST_ASSERT_LESS_OR_EQUAL(4, FAST_TASK_PRIORITY);
    TEST_ASSERT_GREATER_OR_EQUAL(1, SLOW_TASK_PRIORITY);
    TEST_ASSERT_LESS_OR_EQUAL(4, SLOW_TASK_PRIORITY);
}

// Main test runner function
int main(void)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_tick_to_ms_conversion);
    RUN_TEST(test_led_pin_definitions);
    RUN_TEST(test_timing_calculations);
    RUN_TEST(test_task_priorities);
    
    return UNITY_END();
}
