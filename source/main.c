#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "pico/stdlib.h" 
#include "bsp/board.h"
#include "tusb.h"

#include "layers.h"
#include "usb_hid_out_fn.h"

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


    int n_keys = 94; 
    char keys[95 * 2] = {0};
    int i;
    for (int i = 0; i < n_keys; i++) {
        keys[2 * i] = 0;  
        keys[(2 * i) + 1] = out_kwerty[(2 * i) + 1];
        // keys[(2 * i) + 1] = out_dvorak[(2 * i) + 1];
    }
    keys[n_keys * 2] = 0x00;
    keys[(n_keys * 2) + 1] = 0xff;
    send_keycodes(keys);
    
    // char keys[] = {
    //     0x1E, // !
    //     0x00, 0x34, // "
    //     0x00, 0x20, // #
    //     0x00, 0x21, // $
    //     0x00, 0x22, 0x00,
    //     0xff   
    // };
    // // char keys[] = {4, 0, 5, 0, 5, 0, 0xff};
    // send_keycodes(keys);

    led_toggle();
}



int main(void)
{
    board_init();
    tusb_init();

    // #include "pico/multicore.h"
    // multicore_launch_core1(core1_main);
    
    while (1)
    {
        tud_task(); // tinyusb device task
        send_keycodes_task();
    }
}
