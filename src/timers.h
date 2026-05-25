#ifndef TIMERS_H
#define TIMERS_H


#include <avr/io.h>
#include <avr/interrupt.h>
    
extern volatile uint8_t duration_of_the_game;

void init_timer1_ctc();

#endif