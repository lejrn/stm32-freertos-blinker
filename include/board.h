#pragma once

#include <stdint.h>

// STM32F103 Blue Pill LED pins
#define LED_RED    13   // PC13 (built-in LED, active low)
#define LED_GREEN  12   // PB12 (external LED)

// STM32F103 register definitions (simplified)
#define RCC_BASE        0x40021000
#define GPIOB_BASE      0x40010C00
#define GPIOC_BASE      0x40011000

#define RCC_APB2ENR     (*(volatile uint32_t*)(RCC_BASE + 0x18))
#define RCC_APB2ENR_IOPBEN  (1 << 3)  // GPIOB clock enable
#define RCC_APB2ENR_IOPCEN  (1 << 4)  // GPIOC clock enable

// GPIO registers
typedef struct {
    volatile uint32_t CRL;    // Configuration register low
    volatile uint32_t CRH;    // Configuration register high
    volatile uint32_t IDR;    // Input data register
    volatile uint32_t ODR;    // Output data register
    volatile uint32_t BSRR;   // Bit set/reset register
    volatile uint32_t BRR;    // Bit reset register
    volatile uint32_t LCKR;   // Configuration lock register
} GPIO_TypeDef;

#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef*)GPIOC_BASE)

// Function declarations
void board_init(void);
void board_toggle(uint8_t pin);
void board_set_pin(uint8_t pin, uint8_t state);

// System clock configuration
void SystemClock_Config(void);
void SystemInit(void);
