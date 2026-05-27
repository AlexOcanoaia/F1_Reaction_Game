#ifndef TIMERS_H
#define TIMERS_H


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_DS4 311 
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_C5  523

extern volatile uint8_t duration_of_the_game;


void play_music();

void init_timer1_ctc();

#endif