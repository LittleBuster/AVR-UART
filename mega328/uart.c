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

#include "uart.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>


static struct {
    volatile uint8_t data_count;
    volatile uint8_t cmd_ready;
    volatile char data_in[DATA_SZ];
} uart;


void uart_init(void)
{
    uart.data_count = 0;
    uart.cmd_ready = 0;
    memset(uart.data_in, 0x00, DATA_SZ);

    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B |= (1 << RXCIE0);
    sei();
}

void uart_send_byte(char data)
{
    while ((UCSR0A & (1 << UDRE0)) == 0) {};
    UDR0 = data;
}

void uart_send(char *data)
{
    while (*data)
        uart_send_byte(*data++);
}

uint8_t uart_get_data(const char *data)
{
    if (uart.cmd_ready == 1) {
        strcpy(data, uart.data_in);
        uart.cmd_ready = 0;
        return 1;
    } else
        return 0;
}

/*
 * UART callback
 */
ISR (USART_RX_vect)
{
    uart.data_in[uart.data_count] = UDR0;

    if (uart.data_in[uart.data_count] == '\n') {
        uart.cmd_ready = 1;
        uart.data_count = 0;
    } else
        uart.data_count++;
}
