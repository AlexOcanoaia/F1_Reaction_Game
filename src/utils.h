#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define PLAYER1_CHANNEL 0
#define PLAYER2_CHANNEL 1
#define POTENTIOMETER_CHANNEL 2 
#define PLAYER1 1
#define PLAYER2 2

typedef enum {
    STARTING_GAME,
    GAME,
    END_GAME
} GamePhase;


void gpio_init();

void interupts_init();


#endif