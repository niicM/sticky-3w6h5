#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h" 
#include "usb_hid_out_fn.h"
#include "input_right.h"
#include "input_left.h"
#include "input.h"


void setup_input() {
    setup_gpio();
    setup_expander();
}

uint8_t get_is_left(int pos) {
    return (pos < 33) && (pos % 10 < 5);
}

uint8_t get_row(int pos) {
    return pos / 10;
}

uint8_t get_col(int pos) {

    // Or just precompute it?
    // What is faster?
    // uint8_t pre[36] = {0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 4, 3, 2,
    //                    1, 0, 0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 2, 3, 4, 4, 3, 2};
    
    uint8_t is_letter = pos < 30;
    uint8_t is_left = (pos < 33) && (pos % 10 < 5);

    uint8_t rem = pos % 10;
    uint8_t letter_l_col = rem;
    uint8_t letter_r_col = 9 - rem;
    uint8_t letter_col = is_left * letter_l_col + !is_left * letter_r_col;

    uint8_t sym_l_col = rem + 2;
    uint8_t sym_r_col = 7 - rem;
    uint8_t sym_col =  is_left * sym_l_col + !is_left * sym_r_col;

    uint8_t col = is_letter * letter_col + !is_letter * sym_col; 
    return col;
}

void scan(struct key_du_lst* keys) {
    static bool k_down[N_KEYS];

    // char buff[128];
    bool grid_r[ROWS][COLS] = {0};
    bool grid_l[ROWS][COLS] = {0};
    
    keys->n = 0;
    // send_string(".");

    scan_r(grid_r);
    scan_l(grid_l);

    for (int i = 0; i < N_KEYS; i++) {
        uint8_t is_left = get_is_left(i); 
        uint8_t row = get_row(i); 
        uint8_t col = get_col(i);
        bool new = grid_l[row][col] * is_left + grid_r[row][col] * !is_left;
        bool old = k_down[i];
        if (old != new) { 
            struct key_du du = { i, new };
            keys->keys[keys->n] = du;
            keys->n++; 
        }
        k_down[i] = new;
    }
}