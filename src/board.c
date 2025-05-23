#include "board.h"

// External symbol from linker script
extern uint32_t _estack;

// Simple delay function for initialization
static void delay_ms(uint32_t ms)
{
    // Rough delay at 72MHz (not accurate, just for init)
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __asm__("nop");
    }
}

/**
 * @brief Initialize system clock to 72MHz using HSE
 */
void SystemClock_Config(void)
{
    // This is a simplified clock configuration
    // In a real project, you'd use STM32CubeMX generated code
    
    // Enable HSE (8MHz external crystal on Blue Pill)
    volatile uint32_t *RCC_CR = (volatile uint32_t*)(RCC_BASE + 0x00);
    volatile uint32_t *RCC_CFGR = (volatile uint32_t*)(RCC_BASE + 0x04);
    
    *RCC_CR |= (1 << 16);  // HSEON
    while (!(*RCC_CR & (1 << 17))) {} // Wait for HSERDY
    
    // Configure PLL: HSE * 9 = 72MHz
    *RCC_CFGR |= (7 << 18);  // PLLMUL = 9
    *RCC_CFGR |= (1 << 16);  // PLLSRC = HSE
    
    // Enable PLL
    *RCC_CR |= (1 << 24);  // PLLON
    while (!(*RCC_CR & (1 << 25))) {} // Wait for PLLRDY
    
    // Select PLL as system clock
    *RCC_CFGR |= (2 << 0);  // SW = PLL
    while ((*RCC_CFGR & (3 << 2)) != (2 << 2)) {} // Wait for SWS = PLL
}

/**
 * @brief System initialization (called before main)
 */
void SystemInit(void)
{
    // Reset RCC configuration
    volatile uint32_t *RCC_CR = (volatile uint32_t*)(RCC_BASE + 0x00);
    *RCC_CR |= 0x00000001;  // Set HSION
    
    // Configure system clock
    SystemClock_Config();
}

/**
 * @brief Initialize board peripherals
 */
void board_init(void)
{
    // Enable GPIOB and GPIOC clocks
    RCC_APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;
    
    // Small delay to ensure clocks are stable
    delay_ms(1);
    
    // Configure PC13 (built-in LED) as output, push-pull, 2MHz
    // PC13 is in CRH register, position 20-23 (pin 13)
    GPIOC->CRH &= ~(0xF << 20);  // Clear configuration bits
    GPIOC->CRH |= (0x2 << 20);   // Output mode, max speed 2 MHz, push-pull
    
    // Configure PB12 (external LED) as output, push-pull, 2MHz
    // PB12 is in CRH register, position 16-19 (pin 12)
    GPIOB->CRH &= ~(0xF << 16);  // Clear configuration bits
    GPIOB->CRH |= (0x2 << 16);   // Output mode, max speed 2 MHz, push-pull
    
    // Initialize LEDs to OFF state
    // PC13 is active low (built-in LED), so set high to turn off
    GPIOC->BSRR = (1 << LED_RED);
    // PB12 is active high, so set low to turn off
    GPIOB->BRR = (1 << LED_GREEN);
}

/**
 * @brief Toggle a GPIO pin
 * @param pin Pin number to toggle
 */
void board_toggle(uint8_t pin)
{
    if (pin == LED_RED) {
        // PC13 - read current state and toggle
        if (GPIOC->ODR & (1 << pin)) {
            GPIOC->BRR = (1 << pin);   // Set low (LED on, active low)
        } else {
            GPIOC->BSRR = (1 << pin);  // Set high (LED off, active low)
        }
    } else if (pin == LED_GREEN) {
        // PB12 - read current state and toggle
        if (GPIOB->ODR & (1 << pin)) {
            GPIOB->BRR = (1 << pin);   // Set low (LED off)
        } else {
            GPIOB->BSRR = (1 << pin);  // Set high (LED on)
        }
    }
}

/**
 * @brief Set a GPIO pin state
 * @param pin Pin number
 * @param state 1 for high, 0 for low
 */
void board_set_pin(uint8_t pin, uint8_t state)
{
    if (pin == LED_RED) {
        if (state) {
            GPIOC->BSRR = (1 << pin);  // Set high (LED off, active low)
        } else {
            GPIOC->BRR = (1 << pin);   // Set low (LED on, active low)
        }
    } else if (pin == LED_GREEN) {
        if (state) {
            GPIOB->BSRR = (1 << pin);  // Set high (LED on)
        } else {
            GPIOB->BRR = (1 << pin);   // Set low (LED off)
        }
    }
}

// Minimal interrupt vector table
void Reset_Handler(void) __attribute__((weak));
void NMI_Handler(void) __attribute__((weak));
void HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak));
void BusFault_Handler(void) __attribute__((weak));
void UsageFault_Handler(void) __attribute__((weak));
void SVC_Handler(void) __attribute__((weak));
void DebugMon_Handler(void) __attribute__((weak));
void PendSV_Handler(void) __attribute__((weak));
void SysTick_Handler(void) __attribute__((weak));

// Reset handler calls main
void Reset_Handler(void) {
    // Copy data from Flash to RAM
    extern uint32_t _sidata, _sdata, _edata;
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Zero initialize BSS
    extern uint32_t _sbss, _ebss;
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
    
    // Call main
    extern int main(void);
    main();
}

// Default handlers
void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }
void MemManage_Handler(void) { while(1); }
void BusFault_Handler(void) { while(1); }
void UsageFault_Handler(void) { while(1); }
void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}
void SysTick_Handler(void) {}

// Vector table
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)&_estack),  // Stack pointer
    Reset_Handler,                          // Reset Handler
    NMI_Handler,                           // NMI Handler
    HardFault_Handler,                     // Hard Fault Handler
    MemManage_Handler,                     // MPU Fault Handler
    BusFault_Handler,                      // Bus Fault Handler
    UsageFault_Handler,                    // Usage Fault Handler
    0,                                     // Reserved
    0,                                     // Reserved
    0,                                     // Reserved
    0,                                     // Reserved
    SVC_Handler,                           // SVCall Handler
    DebugMon_Handler,                      // Debug Monitor Handler
    0,                                     // Reserved
    PendSV_Handler,                        // PendSV Handler
    SysTick_Handler,                       // SysTick Handler
};
