#pragma once

#include "ringbuf.h"

#define BUFF_SIZE 128
#define USB_DELAY_MS 5

struct element {
    uint8_t mod; uint8_t key;
};

struct print_buff {
    struct element buff[BUFF_SIZE + 1];
    ringbuf_t ring; 
};

void prit_buff_init(struct print_buff* b);
int print_buff_advance(struct print_buff* b);
void print_buff_consume(struct print_buff* b);
void print_buff_add_code(struct print_buff* b, uint8_t mod, uint8_t key);
void print_buff_send_string(struct print_buff* b, char* str);