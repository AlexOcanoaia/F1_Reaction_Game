#include <avr/io.h>
#include "usart.h"
#include "adc.h"
#include <string.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "timers.h"
#include <stdlib.h>

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
    // Button 1 (Start and End)
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    // Player 1 Buttons
    DDRD |= (1 << PD7) | (1 << PD6) | (1 << PD5);
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

uint8_t score1 = 0;
uint8_t score2 = 0;
uint8_t player1_led1 = 0, player1_led2 = 0, player1_led3 = 0;

void turn_led(int player, int led) {
    if (player == 1) {
        if (led == 0) {
            PORTD &= ~((1 << PD6) | (1 << PD5));
            PORTD |= (1 << PD7);
            player1_led1 = 1; player1_led2 = 0; player1_led3 = 0;
        } else if (led == 1) {
            PORTD &= ~((1 << PD7) | (1 << PD5));
            PORTD |= (1 << PD6);
            player1_led1 = 0; player1_led2 = 1; player1_led3 = 0;
        } else {
            PORTD &= ~((1 << PD7) | (1 << PD6));
            PORTD |= (1 << PD5);
            player1_led1 = 0; player1_led2 = 0; player1_led3 = 1;
        }
    }
}

uint8_t player1_pressed = 0;
uint8_t player2_pressed = 0;

#define PLAYER1 1
#define PLAYER2 2

int main() {
    Usart_init(MYUBRR);
    Usart_print("Hello\r\n");
    adc_init();
    gpio_init();
    interupts_init();
    init_timer1_ctc();
    sei();

    char player1_name[20], player2_name[20]; 
    uint8_t read_names = 0;
    uint8_t choice = 0, previous_choice = 0;
    uint8_t previous_time = 0;
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
                        duration_of_the_game = 15;
                        previous_time = 15;
                    } else if (choice == 1) {
                        Usart_print("30 seconds\r\n");
                        duration_of_the_game = 30;
                        previous_time = 30;
                    } else if (choice == 2) {
                        Usart_print("60 seconds\r\n");
                        duration_of_the_game = 60;
                        previous_time = 60;
                    }
                    previous_choice = choice;
                }
            }
            phase = GAME;
            TCCR1B |= (1 << CS12) | (1 << CS10);
        } else if (phase == GAME) {
            if (duration_of_the_game != previous_time) {
                char buffer[20];
                sprintf(buffer, "Time: 00:%u\n", duration_of_the_game);
                Usart_print(buffer);
                previous_time = duration_of_the_game;
            }

            if (!player1_pressed) {
                turn_led(PLAYER1, rand() % 3);
                player1_pressed = 1;
            }

            while (player1_pressed && duration_of_the_game > 0) {
                uint16_t value = read_channel(0);

                if (value > 230 && value < 300 && player1_led1) {
                    PORTD &= ~(1 << PD7);
                    player1_pressed = 0;
                } else if (value > 490 && value < 600 && player1_led2) {
                    PORTD &= ~(1 << PD6);
                    player1_pressed = 0;
                } else if (value > 700 && player1_led3) {
                    PORTD &= ~(1 << PD5);
                    player1_pressed = 0;
                }
            }
            if (!player1_pressed) {
                score1++;
            }
            char buffer_score[20];
            sprintf(buffer_score, "Score: %u\n", score1);
            Usart_print(buffer_score);
            if (duration_of_the_game == 0) {
                phase = END_GAME;
                TCCR1B &= ~((1 << CS12) | (1 << CS10));
            }
        } else if (phase == END_GAME) {
            Usart_print("The End of the game\n");

        }
    }
    return 0;
}