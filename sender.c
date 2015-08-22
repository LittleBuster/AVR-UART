/* AVR MAX485 and USART example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define DATA_SZ 50


void usart_send_byte(char data)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {};
	UDR0 = data;
}

void usart_send(char *data)
{
	while (*data) {
		usart_send_byte(*data++);
	}
}

int main()
{
	char data[DATA_SZ];

	/* USART init */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	UCSR0B |= (1 << RXCIE0 );

	/* example of temperature and humidity data */
	memset(data, 0x00, DATA_SZ);
	strcpy(data, "22.3$90.0\n");

	for (;;) {
		_delay_ms(1000);
		usart_send(data);
	}
}