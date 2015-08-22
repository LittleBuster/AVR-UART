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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdint.h>


#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define DATA_SZ 50


volatile uint8_t data_count;
volatile uint8_t cmd_ready;
volatile char data_in[DATA_SZ];

struct sensor_data {
    char temp[10];
    char hum[10];
};

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

void print_data(struct sensor_data *sdata)
{
    usart_send("=====================\n");
    usart_send("Temp: ");
    usart_send(sdata->temp);
    usart_send_byte('\n');
    usart_send("Hum: ");
    usart_send(sdata->hum);
    usart_send_byte('\n');
    usart_send("=====================\n");
}

void parse_data(struct sensor_data *sdata, const char *data)
{
    uint8_t i = 0;
    char *val = strtok(data, "$");

    while(val != NULL) {
        val = strtok(NULL, "$");
        switch (i) {
            case 0: {
                strcpy(sdata->hum, val);
                break;
            }
            case 1: {
                strcpy(sdata->temp, val);
                break;
            }
        }
        i++;
    }
}

int main()
{
    struct sensor_data sdata;

    /* USART init */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B |= (1 << RXCIE0 );
    sei();

    memset(data_in, 0x00, DATA_SZ);

    for (;;) {
        _delay_ms(1000);

        if (cmd_ready) {
            cmd_ready = 0;
            parse_data(&sdata, data_in);
            print_data(&sdata);
            memset(data_in, 0x00, DATA_SZ);
        }
    }
    return 0;
}

ISR (USART_RX_vect)
{
    data_in[data_count] = UDR0;

    if (data_in[data_count] == '\n') {
        cmd_ready = 1;
        data_count = 0;
    } else {
        data_count++;
    }
}