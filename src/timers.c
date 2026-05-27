#include "timers.h"

volatile uint8_t duration_of_the_game;

uint16_t f1_song[] =  {
        NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_DS4, NOTE_F4,
        NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_G4,  NOTE_GS4,
        NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_DS4, NOTE_F4,

        NOTE_C5,
        NOTE_G4,
        NOTE_GS4,
        NOTE_DS4, NOTE_F4, NOTE_G4,
        NOTE_C4,
        0
    };

uint16_t f1_durations[] = {
    100, 100, 100, 100, 200, 200,
    100, 100, 100, 100, 200, 200,
    100, 100, 100, 100, 200, 200,

    1200,
    1200,
    1200,
    400, 400, 400,
    1600,
    
    2000
}; 

void init_timer1_ctc() {
    TCCR1B |= (1 << WGM12); // starting the ctc mode

    // 1 second
    OCR1A = 15624;

    // To turn on the interuptions on the OCR1A
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    if (duration_of_the_game > 0) {
        duration_of_the_game--;
    }
}

void delay(uint16_t time) {
    while (time) {
        _delay_ms(1);
        time--;
    }
}

void playNote(uint16_t freq, uint16_t duration) {
    if (!freq) {
        TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
        delay(duration);
        return;
    }

    uint32_t tmp = (F_CPU / (512UL * freq)) - 1;
    OCR2A = (uint8_t) (tmp > 255 ? 255 : tmp);

    TCCR2A = (1 << COM2A0) | (1 << WGM21);
    TCCR2B = (1 << CS22) | (1 << CS21);

    delay(duration);
    TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    TCCR2A = 0;
}

void play_music() {
    uint8_t length = sizeof(f1_song) / sizeof(f1_song[0]);
    for (uint8_t i = 0; i < length; i++) {
        playNote(f1_song[i], f1_durations[i]);
        _delay_ms(20);
    }
}