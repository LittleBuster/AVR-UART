/* UART atmega328p AVR library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define DATA_SZ 50

/*
 * UART initialization
 */
void uart_init(void);

/**
 * Sending byte to uart
 * @data: byte
 */
void uart_send_byte(char data);

/**
 * Sending byte massive to uart
 * @data: byte massive
 */
void uart_send(char *data);

/**
 * Get data from UART interrupt
 * @data: byte massive. size = 50
 */
uint8_t uart_get_data(const char *data);


#endif
