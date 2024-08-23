#include <stdint.h>

#include "pico/stdlib.h" 
#include "tusb.h"

// #include "layers.h"
#include "layouts.h"
#include "print_buff.h"
#include "usb_hid_out_fn.h"


void prit_buff_init(struct print_buff* b) {
    ringbuf_init(
        &b->ring, (uint8_t*) b->buff,  BUFF_SIZE, sizeof(struct element));
}


int print_buff_advance(struct print_buff* b) {
    
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
    sleep_ms(USB_DELAY_MS);
    tud_task();

    tud_hid_keyboard_report(1, mod, release);
    sleep_ms(USB_DELAY_MS);
    tud_task();

    return i;
}


void print_buff_consume(struct print_buff* b) {
    
    while (print_buff_advance(b) != 0) {
        // send_string("-");
        sleep_ms(USB_DELAY_MS);
    }
}


void print_buff_add_code(struct print_buff* b, uint8_t mod, uint8_t key) {
    struct element el = {mod, key};
    ringbuf_put(&b->ring, &el);
}


void print_buff_send_string(struct print_buff* b, char* str) {
    int i;
    for (i = 0; ; i++) {
        if (str[i] == 0) break;
        int pos = str[i] - ' ';        
        uint8_t mod = out_kwerty[2 * pos];
        uint8_t key = out_kwerty[(2 * pos) + 1];

        struct element el = {mod, key};
        ringbuf_put(&b->ring, &el);
    }
}