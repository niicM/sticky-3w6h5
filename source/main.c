#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "pico/stdlib.h" 
#include "bsp/board.h"

#include "tusb.h"

// #include "layers.h"
#include "usb_hid_out_fn.h"
#include "input.h"


void led_toggle(void)
{
    static bool led_state = 0;
    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

////////////////////////////////////////////////////////////////////////////////
//  Main code


void send_keycodes_task() {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    scan();
    led_toggle();
}

bool scan_callback(struct repeating_timer *t) {
    scan();
    return true;
}

int main(void) {

    board_init();
    tusb_init();
    stdio_init_all();
    
    setup_input();
    // #include "pico/multicore.h"
    // multicore_launch_core1(core1_main);
    
    struct repeating_timer timer;
    add_repeating_timer_ms(50, scan_callback, NULL, &timer);
    while (1)
    {
        tud_task(); // tinyusb device task
        send_keycodes_task();
    }
}

// static bool add_repeating_timer_ms(
//     int32_t delay_ms, repeating_timer_callback_t callback, 
//     void *user_data, repeating_timer_t *out)