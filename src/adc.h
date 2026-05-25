#ifndef ADC_H
#define ADC_H

#include <stdio.h>
#include <avr/io.h>

void adc_init();

uint16_t read_channel(uint8_t channel);

#endif