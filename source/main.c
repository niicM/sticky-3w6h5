#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h" 
#include "pico/multicore.h"
#include "bsp/board.h"

#include "tusb.h"

// #include "layers.h"
#include "usb_hid_out_fn.h"
#include "input.h"
#include "print_buff.h"


void led_toggle(void)
{
    static bool led_state = 0;
    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

void send_keycodes_task() {
    const uint32_t interval_ms = 50;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    struct key_du_lst keys;
    scan(&keys);


    char buff[128];
    for (int i = 0; i < keys.n; i++) {
        int code = keys.keys[i].code;
        if (keys.keys[i].is_down) {
            sprintf(buff, " d(%02d) ", code);
        } else {
            sprintf(buff, " u(%02d) ", code);
        }
        send_string(buff);
    }
    
    led_toggle();
}


void test_task(struct print_buff* pb) {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;
    

    // send_string("o ");
    print_buff_send_string(pb, "hey: Having one declaration. ");
    // print_buff_advance(pb);
    print_buff_consume(pb);
    led_toggle();
}


int main(void) {

    board_init();
    tusb_init();
    stdio_init_all();
    setup_input();
    
    struct print_buff pb;
    prit_buff_init(&pb);

    while (1)
    {
        tud_task(); // tinyusb device task
        test_task(&pb);
        // send_keycodes_task();
        // sleep_ms(1000);
    }
}
