#include "FreeRTOS.h"
#include "task.h"
#include "board.h"

/**
 * @brief Fast blink task - toggles red LED every 200ms
 * @param pvParameters Task parameters (unused)
 */
static void blink_fast_task(void *pvParameters)
{
    (void)pvParameters; // Suppress unused parameter warning
    
    for (;;) {
        board_toggle(LED_RED);
        vTaskDelay(pdMS_TO_TICKS(200)); // 200ms delay = 5Hz blink rate
    }
}

/**
 * @brief Slow blink task - toggles green LED every 1000ms
 * @param pvParameters Task parameters (unused)
 */
static void blink_slow_task(void *pvParameters)
{
    (void)pvParameters; // Suppress unused parameter warning
    
    for (;;) {
        board_toggle(LED_GREEN);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1000ms delay = 1Hz blink rate
    }
}

/**
 * @brief Application entry point
 * @return Should never return
 */
int main(void)
{
    // Initialize board hardware
    board_init();
    
    // Create fast blink task (higher priority)
    xTaskCreate(blink_fast_task, 
                "FastBlink", 
                configMINIMAL_STACK_SIZE, 
                NULL, 
                2,  // Priority 2 (higher)
                NULL);
    
    // Create slow blink task (lower priority)
    xTaskCreate(blink_slow_task, 
                "SlowBlink", 
                configMINIMAL_STACK_SIZE, 
                NULL, 
                1,  // Priority 1 (lower)
                NULL);
    
    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
    
    // Should never reach here if scheduler starts successfully
    for (;;) {
        // Error: scheduler failed to start
    }
    
    return 0;
}
