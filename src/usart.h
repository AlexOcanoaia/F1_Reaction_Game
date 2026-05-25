#ifndef USART_H
#define USART_H

#include <stdio.h>
#include <avr/io.h>

#define CLOCK_SPEED 16000000
#define BAUD 57600
#define MYUBRR CLOCK_SPEED/16/BAUD-1

void Usart_init(unsigned int ubbr);

void Usart_transmit(char data);

char Usart_receive();

void Usart_print(const char *data);


#endif