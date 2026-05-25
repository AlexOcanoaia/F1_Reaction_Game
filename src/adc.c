#include "adc.h"

void adc_init() {
    ADMUX = 0;
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN);

    // Set the prescaler
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_channel(uint8_t channel) {
    channel &= 0b00000111;
    ADMUX &= 0b11111000;
    ADMUX |= channel;
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC));
    return ADC;
}
