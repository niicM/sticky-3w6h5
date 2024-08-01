#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h" 

#include "usb_descriptors.h"
#include "usb_hid_out.h"


void send_keycodes_task() {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time

    // led_toggle();
    start_ms += interval_ms;
    char keys[] = {4, 0, 5, 0, 6, 0, 0xff};
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
