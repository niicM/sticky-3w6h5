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

#include "e_effects.h"
#include "e_press_to_effect.h"

void led_toggle(void)
{
    static bool led_state = 0;
    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

void send_keycodes_task(struct press_to_effect* pte, struct print_buff* pb) {
    
    const uint32_t interval_ms = 50;
    static uint32_t current_ms = 0;
    if (board_millis() - current_ms < interval_ms)
        return; // not enough time
    current_ms += interval_ms;
    /////////////////////////////////////////

    struct effect ef;
    
    struct key_du_lst keys;
    scan(&keys);


    char buff[128];
    for (int i = 0; i < keys.n; i++) {
        int code = keys.keys[i].code;
        if (keys.keys[i].is_down) {
            sprintf(buff, " d(%02d) ", code);        
            // send_string(buff);

            key_down(pte, &ef, code);
        } else {
            sprintf(buff, " u(%02d) ", code);
            // send_string(buff);
            
            key_up(pte, &ef, code);
            // sprintf(buff, " %c ", ef.payload);
            if (ef.effect_type == ASCII_TYPE) {
                sprintf(buff, "%c", ef.payload);
                send_string(buff);
            } else {
                send_keycode(0, ef.payload, true);
            }
        }
    }

    led_toggle();
}


void test_task(struct print_buff* pb) {
    const int interval_ms = 2000;
    static int start_ms = 2010;
    if (board_millis() < start_ms + interval_ms) return; // not enough time
    start_ms += interval_ms;
    

    // send_string("o ");
    // print_buff_send_string(pb, "hey: Having one declaration. ");
    
    // char buff[128];
    // sprintf(buff, " m: %d, s: %d, ", board_millis(), start_ms);
    // send_string(buff);

    print_buff_send_string(pb, "Hello");
    // print_buff_advance(pb);
    // print_buff_consume(pb);

    led_toggle();
}


int main(void) {

    board_init();
    tusb_init();
    stdio_init_all();
    setup_input();
    
    // Use this for more optimized (usb) printing
    struct print_buff pb;
    prit_buff_init(&pb);

    struct press_to_effect pte;
    init_press_to_effect(&pte);
    
    while (1)
    {
        tud_task(); // tinyusb device task
        
        test_task(&pb);
        // send_keycodes_task(&pte, &pb);
        
        advance_task(&pb);

        // sleep_ms(1000);
    }
}
