#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h" 
#include "bsp/board.h"
#include "tusb.h"

#include "layers.h"
#include "usb_hid_out_fn.h"



////////////////////////////////////////////////////////////////////////////////
//  Keyboard output

// Approach one using calbacks. Too complicated compared to the simple approach
/*
#define BUFFER_SIZE 256

char send_buffer[BUFFER_SIZE] = {0xff};
int send_buffer_idx = -1;

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
*/

void send_keycode(char mod, char key, bool first) {
    uint8_t report[6];

    if (first) {
        memset(report, 0, sizeof(report));
        tud_hid_keyboard_report(1, 0, report);
        sleep_ms(4);
        tud_task();
    }    

    memset(report, 0, sizeof(report));
    report[0] = key;
    tud_hid_keyboard_report(1, mod, report);
    sleep_ms(4);
    tud_task();

    memset(report, 0, sizeof(report));
    tud_hid_keyboard_report(1, 0, report);
    sleep_ms(4);
    tud_task();

}

void send_keycodes(char* mods, char* keys) {

    send_keycode(mods[0], keys[0], true);

    for(int i = 1; ; i++) {
        if (keys[i] == 0xff) break;
        send_keycode(mods[i], keys[i], false);
    }
}

void send_string(char* str) {
    int len = strlen(str);
    char mods[len + 1];
    char keys[len + 1];
    int i;
    for (i = 0; ; i++) {
        if (str[i] == 0) break;
        int pos = str[i] - ' ';
        mods[i] = out_kwerty[2 * pos];
        keys[i] = out_kwerty[(2 * pos) + 1];
    }
    keys[i] = 0xff;
    // keys[len] = 0xff;

    send_keycodes(mods, keys);
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len) { }

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, 
        hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, 
        hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}
