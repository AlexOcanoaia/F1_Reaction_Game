#include "timers.h"

void init_timer1_ctc() {
    TCCR1B |= (1 << WGM12); // starting the ctc mode

    // 1 second
    OCR1A = 15624;

    // To turn on the interuptions on the OCR1A
    TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    if (duration_of_the_game >= 0) {
        duration_of_the_game--;
    }
}