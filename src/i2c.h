#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "usart.h"
#include <util/twi.h>
#include <util/delay.h>

#define TWBR_VAL ((16000000UL / 100000UL - 16) / 2)
#define MAX_TIME 20000UL

void i2c_init();

uint8_t i2c_wait();

void i2c_reset();

uint8_t i2c_start(uint8_t address);

void i2c_write(uint8_t data);

void i2c_stop();

#endif