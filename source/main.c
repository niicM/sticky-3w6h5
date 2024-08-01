#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h" 

#include "usb_descriptors.h"

#define BUFFER_SIZE 256

char send_buffer[BUFFER_SIZE] = {0xff};
int send_buffer_idx = -1;


void led_toggle(void);
void hid_task(void);
void send_keycodes(char* keys);

void send_keycodes_task() {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time

    led_toggle();
    start_ms += interval_ms;
    char keys[] = {4, 0, 5, 0, 6, 0, 0xff};
    send_keycodes(keys);
}

/*------------- MAIN -------------*/
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

void send_keycodes(char* keys) {
    // If we are still sending previous keycodes
    if (send_buffer_idx != -1) return;
    send_buffer_idx++;

    // Copy over to global buffer
    for(int i = 0; i < BUFFER_SIZE; i++) {
        if (keys[i] == 0xff) break;
        send_buffer[i] = keys[i];
    }

    // Make an initial empty report to start the callback chain
    uint8_t keycode[6] = {0};
    tud_hid_keyboard_report(1, 0, keycode);
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
    (void)instance;
    (void)len;
    (void)report;

    if (send_buffer_idx < BUFFER_SIZE && send_buffer[send_buffer_idx] != 0xff) {
        uint8_t keycode[6] = {0};
        keycode[0] = send_buffer[send_buffer_idx];
        tud_hid_keyboard_report(1, 0, keycode);
        send_buffer[send_buffer_idx] = 0xff;
        send_buffer_idx++;
    } else {
        send_buffer_idx = -1;
    }
}

void led_toggle(void)
{
    static bool led_state = 0;
    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}
