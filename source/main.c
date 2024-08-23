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


void led_toggle(void)
{
    static bool led_state = 0;
    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

////////////////////////////////////////////////////////////////////////////////
//  Main code


void send_keycodes_task() {
    const uint32_t interval_ms = 50;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    scan();
    led_toggle();
}

// bool scan_callback(struct repeating_timer *t) {
//     send_string(":");
//     led_toggle();
//     // scan();

//     return true;
// }

// void core1_main() {

//     // struct repeating_timer timer;
//     // add_repeating_timer_ms(1000, scan_callback, NULL, &timer);
//     while (1) {
//         send_keycodes_task();
//         // sleep_ms(500);
//     }
// }

int main(void) {

    board_init();
    tusb_init();
    stdio_init_all();
    setup_input();

    // multicore_launch_core1(core1_main);
    
    while (1)
    {
        tud_task(); // tinyusb device task
        send_keycodes_task();
        // sleep_ms(500);
    }
}

// static bool add_repeating_timer_ms(
//     int32_t delay_ms, repeating_timer_callback_t callback, 
//     void *user_data, repeating_timer_t *out)