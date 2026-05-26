#include "i2c.h"

void i2c_init() {
    TWBR = 72;

    // Set the prescaler to 1
    TWSR = 0x00;

    TWCR |= (1 << TWEN);
}

// Returns 0 if the time is gone
uint8_t i2c_wait() {
    uint32_t tmp = MAX_TIME;
    while (!(TWCR & (1 << TWINT))) {
        tmp--;
        if (tmp == 0)
            return 0;
    }
    return 1;
}

void i2c_reset() {
    TWCR = 0;
    _delay_us(10);
    TWCR = (1 << TWEN);
}

uint8_t i2c_start(uint8_t address) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    if (i2c_wait() == 0) {
        i2c_reset();
        return 0;
    }

    uint8_t s = TWSR & 0xF8;
    if (s != TW_START && s != TW_REP_START) {
        i2c_reset();
        return 0;
    }

    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);

    if (i2c_wait() == 0) {
        i2c_reset();
        return 0;
    }

    s = TWSR & 0xF8;
    if (s != TW_MT_SLA_ACK && s != TW_MR_SLA_ACK)
        return 0;
    return 1;
}

void i2c_write(uint8_t data) {
    TWDR = data;

    TWCR = (1 << TWINT) | (1 << TWEN);

    if (i2c_wait() == 0)
        i2c_reset();
}

void i2c_stop() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    while ((TWCR & (1 << TWSTO)));
}
