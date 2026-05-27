#include "utils.h"

void gpio_init() {
    // Button 1 (Start and End)
    DDRB &= ~(1 << PB0);
    PORTB |= (1 << PB0);

    // Player 1 Buttons
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5);
    
    // Player 2 buttons
    DDRD |= (1 << PD4) | (1 << PD3) | (1 << PD2);

    DDRB |= (1 << PB3);
}

void interupts_init() {
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);
}
