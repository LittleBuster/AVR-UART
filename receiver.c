/* Example of transfer data by rs485 for tests
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <DHT.h>
#include <EasyTransfer.h>


#define CLIENT_1 100

#define GET_INFO 111 
#define ANSW_OK 222

struct send_data {
    unsigned id;
    unsigned data;
    
    union sensor_t {
      float temp;
      float hum;
    } sensor;
};

struct recv_data {
    unsigned id;
    unsigned cmd;
};

struct sender {
    EasyTransfer et;
    struct send_data data;
};

/* Main struct */
struct receiver {
    
    EasyTransfer et;
    struct recv_data data;
    struct sender snd;
};

/**
 * Send data to other client by rs845
 * @snd: data sender
 * @id: id of current client (needs for many clients)
 * @data: example of data which sending to other client
 */
void sender_send_data(struct sender *snd, unsigned id, unsigned data)
{
    snd->data.id = id;
    snd->data.data = data;
    snd->et.begin(details(snd->data), &Serial);
    snd->et.sendData();
}

/* 
 * Initialization of Easy Transfer lib with my data structure
 */
void receiver_init(struct receiver *recv)
{
    recv->et.begin(details(recv->data), &Serial);    
}

/*
 * Receiving data from other client and parse it
 * After sends answ
 */
void receiver_recv_data(struct receiver *recv)
{
    unsigned i;
    
    if (recv->et.receiveData()) {
        if (recv->data.id == CLIENT_1) {
            if (recv->data.cmd == GET_INFO) {
                /*
                 * Led blinking 3 times
                 */
                for (i = 0; i < 3; i++) {
                    digitalWrite(9, HIGH);
                    delay(500);
                    digitalWrite(9, LOW);
                    delay(500);
                }
                sender_send_data(&recv->snd, CLIENT_1, ANSW_OK);
            }
        }
    }
}

void receiver_get_sensor_data(struct receiver *recv)
{
    DHT dht(10, DHT22);
    dht.begin();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (isnan(t) || isnan(h)) {
      recv->snd.data.sensor.temp = t;
      recv->snd.data.sensor.hum = h;
    }
}

struct receiver recv;

void setup()
{
    Serial.begin(9600);
    receiver_init(&recv);
    pinMode(9, OUTPUT);
}

void loop()
{
    receiver_get_sensor_data(&recv);
    receiver_recv_data(&recv);    
}
