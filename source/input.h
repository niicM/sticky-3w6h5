#pragma once

#include <stdint.h>
#include <stdbool.h>

#define ROWS 4
#define COLS 5
#define N_KEYS 36

struct key_du {
    uint8_t code;
    bool is_down;
};

struct key_du_lst {
    struct key_du keys[N_KEYS];
    int n;
};

void setup_input();

void scan(struct key_du_lst* keys);
