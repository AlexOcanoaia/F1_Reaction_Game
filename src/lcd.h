#ifndef LCD_H
#define LCD_H


#include <avr/io.h>
#include "usart.h"
#include "i2c.h"
#include <util/delay.h>
#include <util/twi.h>

#define LCD_ADDRESS 0x27
#define LCD 0x08
#define ENABLE 0x04
#define RS 0x01

void write(uint8_t data);

void lcd_pulse(uint8_t data);

void lcd_send_nibble(uint8_t nibble, uint8_t flags);

void lcd_send_byte(uint8_t byte, uint8_t flags);

void lcd_cmd(uint8_t cmd);

void lcd_char(char data);

void lcd_set_cursor(uint8_t row, uint8_t col);

void lcd_print(char *s);

void lcd_init();
#endif