#include "lcd.h"

void write(uint8_t data) {
    if (i2c_start(LCD_ADDRESS << 1)) {
        i2c_write(data);
    }
    i2c_stop();
}

void lcd_pulse(uint8_t data) {
    write(data & ~ENABLE);
    _delay_us(1);

    write(data | ENABLE);
    _delay_us(20);
    write(data & ~ENABLE);
    _delay_us(50);
}

void lcd_send_nibble(uint8_t nibble, uint8_t flags) {
    uint8_t send = nibble & 0xF0;
    lcd_pulse(send | flags | LCD);
}

void lcd_send_byte(uint8_t byte, uint8_t flags) {
    uint8_t first = byte & 0xF0;
    uint8_t second = (byte << 4) & 0xF0;

    lcd_send_nibble(first, flags);
    lcd_send_nibble(second, flags);
}

void lcd_cmd(uint8_t cmd) {
    lcd_send_byte(cmd, 0);
}

void lcd_char(char data) {
    lcd_send_byte((uint8_t) data, RS);
}

uint8_t row_offsets[] = {0x00, 0x40};

void lcd_set_cursor(uint8_t row, uint8_t col) {
    lcd_cmd(0x80 | (row_offsets[row] + col));
    _delay_us(50);
}


void lcd_print(char *s) {
    while (*s != '\0') {
        lcd_char(*s);
        s++;
    }
}

void lcd_init() {
    _delay_ms(50);                    

    lcd_send_nibble(0x30, 0); 
    _delay_ms(5);
    lcd_send_nibble(0x30, 0);
     _delay_ms(1);
    lcd_send_nibble(0x30, 0);
    _delay_ms(1);

    lcd_send_nibble(0x20, 0);
    _delay_ms(1);

    lcd_cmd(0x28);
    _delay_ms(1);   
    lcd_cmd(0x08);
    _delay_ms(1);   
    lcd_cmd(0x01);
    _delay_ms(2);
    lcd_cmd(0x06);
    _delay_ms(1);
    lcd_cmd(0x0C);
    _delay_ms(1);
}

void lcd_clear() {
    lcd_cmd(0x01);
    _delay_ms(2);
}
