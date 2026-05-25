#include <avr/io.h>
#include "usart.h"
#include "adc.h"
#include <string.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define POTENTIOMETER_CHANNEL 1 // ADC1 (PC1)

typedef enum {
    STARTING_GAME,
    GAME,
    END_GAME
} GamePhase;

GamePhase phase = STARTING_GAME;

/**
 * ADC values is [0, 1023],
 * so for the potentiometer will have this next 3 intervals:
 * [0, 341] -> 15 seconds
 * [342, 682] -> 30 seconds
 * [683, 1023] -> 60 seconds
 */

void gpio_init() {
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
}

void interupts_init() {
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);
}

volatile uint8_t is_game_starting = 0;

ISR(PCINT2_vect) {
    if (!(PIND & (1 << PD2))) {
        if (phase == STARTING_GAME) {
            is_game_starting = 1;
            // _delay_ms(100);
        }
    }
}

int main() {
    Usart_init(MYUBRR);
    Usart_print("Hello\r\n");
    adc_init();
    gpio_init();
    interupts_init();
    sei();

    char player1_name[20], player2_name[20]; 
    uint8_t read_names = 0;
    uint8_t choice = 0, previous_choice = 0;
    while (1) {
        if (phase == STARTING_GAME) {
            if (!read_names) {
                Usart_print("Enter Player 1 name: ");
                Usart_read_string(player1_name, 20);
                Usart_print("\n");

                Usart_print("Enter Player 2 name: ");
                Usart_read_string(player2_name, 20);
                Usart_print("\n");

                read_names = 1;
            }
            
            while (!is_game_starting) {
                uint16_t result = read_channel(POTENTIOMETER_CHANNEL);

                if (result <= 320) {
                    choice = 0;
                } else if (result > 380 && result <= 682) {
                    choice = 1;
                } else if (result >= 710) {
                    choice = 2;
                }

                if (choice != previous_choice) {
                    if (choice == 0) {
                        Usart_print("15 seconds\r\n");
                    } else if (choice == 1) {
                        Usart_print("30 seconds\r\n");
                    } else if (choice == 2) {
                        Usart_print("60 seconds\r\n");
                    }
                    previous_choice = choice;
                }
            }
            phase = GAME;
        } else if (phase == GAME) {
            Usart_print("Starting game\r\n");
        }
    }
    return 0;
}