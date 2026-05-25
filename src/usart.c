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

void Usart_read_string(char *buf, int length) {
    if (!buf)
        return;

    char data;
    int l = 0;
    while (1) {
        data = Usart_receive();

        if ((data == '\r' || data == '\n') && l == 0)
            continue;

        if (data == '\n' || data == '\r') {
            buf[l++] = '\0';
            return;
        }

        if (l < (length - 1)) {
            buf[l++] = data;
        }
        Usart_transmit(data);
    }
}
