#include "i2c.h"


void i2c_init() {
    PORTC |= (1 << SDA) | (1 << SCL);
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));

    TWBR = TWBR_VAL;
    TWCR = 0;
    TWCR = (1 << TWEN);
}

void i2c_start() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

    while (!(TWCR & (1 << TWINT)));
}

void i2c_write(uint8_t data) {
    TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}