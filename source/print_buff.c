#include <stdint.h>

#include "pico/stdlib.h" 
#include "bsp/board.h"
#include "tusb.h"

// #include "layers.h"
#include "layouts.h"
#include "print_buff.h"
#include "usb_hid_out_fn.h"


void prit_buff_init(struct print_buff* b) {
    ringbuf_init(
        &b->ring, (uint8_t*) b->buff,  BUFF_SIZE, sizeof(struct element));
}


int print_buff_advance(struct print_buff* b, bool complete, uint8_t* out_mod) {
    
    uint8_t report[6] = {
        0, 0, 0, 0, 0, 0
    };
    
    uint8_t release[6] = {
        0, 0, 0, 0, 0, 0
    };

    struct element el;
    
    uint8_t mod = 0;
    bool cont = ringbuf_peek(&b->ring, &el);
    if (!cont) {
        return 0;
    } else {
        mod = el.mod;
    }

    int i;
    for (i = 0; i < 6; i++) {
        cont = ringbuf_peek(&b->ring, &el);
        if (!cont) break;
        if (el.mod != mod) break;

        bool repeated = false;
        for (int j = 0; j < i; j++) {
            repeated |= el.key == report[j];
        }
        if (repeated) break;
        
        cont = ringbuf_get(&b->ring, &el);
        report[i] = el.key;
    }

    tud_hid_keyboard_report(1, mod, report);

    if (complete) {
        sleep_ms(USB_DELAY_MS);
        tud_task();

        tud_hid_keyboard_report(1, mod, release);
        sleep_ms(USB_DELAY_MS);
        tud_task();
    }

    if (out_mod) {
        *out_mod = mod;
    }

    return i;
}


void print_buff_consume(struct print_buff* b) {
    
    while (print_buff_advance(b, true, NULL) != 0) {
        // send_string("-");
        // sleep_ms(USB_DELAY_MS);
    }
}


void print_buff_add_code(struct print_buff* b, uint8_t mod, uint8_t key) {
    struct element el = {mod, key};
    ringbuf_put(&b->ring, &el);
}

struct element char_to_el(char c) {
    int pos = c - ' ';        
    uint8_t mod = out_kwerty[2 * pos];
    uint8_t key = out_kwerty[(2 * pos) + 1];
    struct element el = {mod, key};
    return el;
}

void print_buff_send_string(struct print_buff* b, char* str) {
    int i;
    for (i = 0; ; i++) {
        char c = str[i];
        if (c == 0) break;
        struct element el = char_to_el(c);
        ringbuf_put(&b->ring, &el);
    }
}


void print_buff_send_char(struct print_buff* b, char c) {
    struct element el = char_to_el(c);
    ringbuf_put(&b->ring, &el);
}


void print_buff_send_key_code(struct print_buff* b, uint8_t mod, uint8_t key_code) {
    struct element el = {mod, key_code};
    ringbuf_put(&b->ring, &el);
}


void advance_task(struct print_buff* b) {

    static bool need_cleaning = true;
    static uint8_t last_mod = 0;

    const int interval_ms = 10;
    static int current_ms = 1000;
    int now = board_millis();
    if (now < current_ms + interval_ms)
        return; // not enough time
    // current_ms += interval_ms;
    current_ms = now;
    /////////////////////////////////////////

    // The pattern:
    // 1 tud_task
    // 2 send_report with keys
    // 3 sleep USB_DELAY_MS
    // 4 tud_task
    // 5 send_report with all zeros
    // 6 sleep USB_DELAY_MS
    // (repeat)

    tud_task();
    if (need_cleaning) {
        uint8_t release[6] = { 0, 0, 0, 0, 0, 0 };
        tud_hid_keyboard_report(1, last_mod, release);
        need_cleaning = false;
    }
    else {
        // last_print_count = print_buff_advance(b, false, &last_mod); 
        int c = print_buff_advance(b, false, &last_mod);
        if (c) need_cleaning = true;
    }
}
