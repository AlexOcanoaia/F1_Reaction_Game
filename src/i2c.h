#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdio.h>
#include "usart.h"

#define SDA PC4
#define SCL PC5
#define F_CPU 16000000UL
#define F_SCL 100000UL

#define TWBR_VAL (((F_CPU / F_SCL) - 16) / 2)

void i2c_init();

void i2c_start();

void i2c_write();

void i2c_stop();


#endif