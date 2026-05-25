#include "usart.h"

void Usart_init(unsigned int ubbr) {
    UBRR0H = (unsigned char) (ubbr >> 8);
    UBRR0L = (unsigned char) (ubbr);

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    UCSR0C = (3 << UCSZ00);
}

void Usart_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

char Usart_receive() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void Usart_print(const char *data) {
    while (*data != '\0') {
        Usart_transmit(*data);
        data++;
    }
}
