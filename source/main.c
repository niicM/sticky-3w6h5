#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "pico/stdlib.h" 
#include "bsp/board.h"
#include "tusb.h"

#include "usb_hid_out_fn.h"
#include "layers.h"

void send_keycodes_task() {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time


    char keys[95 * 2] = {0};
    for (int i = 0; i < 94; i++) {
        keys[2 * i] = 0;  
        keys[(2 * i) + 1] = out_kwerty[(2 * i) + 1];
    }
    
    // led_toggle();
    // start_ms += interval_ms;
    // char keys[] = {4, 0, 5, 0, 6, 0, 0xff};
    send_keycodes(keys);
}



int main(void)
{
    board_init();
    tusb_init();
    while (1)
    {
        tud_task(); // tinyusb device task
        send_keycodes_task();
    }
}
