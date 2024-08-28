#pragma once

#include "ringbuf.h"

#define BUFF_SIZE 511
#define USB_DELAY_MS 5

struct element {
    uint8_t mod; uint8_t key;
};

struct print_buff {
    struct element buff[BUFF_SIZE + 1];
    ringbuf_t ring; 
};

void prit_buff_init(struct print_buff* b);
int print_buff_advance(struct print_buff* b, bool complete, uint8_t* out_mod);
void print_buff_consume(struct print_buff* b);
void print_buff_send_string(struct print_buff* b, char* str);
void print_buff_send_char(struct print_buff* b, char c);
void print_buff_send_char_w_mod(struct print_buff* b, uint8_t mod, char c);
void print_buff_send_key_code(struct print_buff* b, uint8_t mod, uint8_t key);
void advance_task(struct print_buff* b);