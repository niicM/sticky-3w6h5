/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h" 

#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum
{
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED = 1000,
    BLINK_SUSPENDED = 2500,
};


char send_buffer[64] = {0xff};
int send_buffer_idx = 0;

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);
void send_keycodes(char* keys);

void send_keycodes_task() {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time

    led_blinking_task();
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
        // char keys[] = {4, 5, 6, 0xff};
        // send_keycodes(keys);

        // hid_task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
    // blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    // blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    // (void)remote_wakeup_en;
    // blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    // blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+


// static void send_hid_report_(uint8_t report_id, uint32_t btn)
// {
//     // skip if hid is not ready yet
//     if (!tud_hid_ready())
//         return;

//     if (btn)
//     {
        
//         uint8_t keycode_a[6] = {0};
//         keycode_a[0] = HID_KEY_C;
//         tud_hid_keyboard_report(1, 0, keycode_a);
//         board_delay(5);
        
//         uint8_t zeros[6] = {0};
//         tud_hid_keyboard_report(1, 0, zeros);
//         board_delay(5);
        
//         uint8_t keycode_b[6] = {0};
//         keycode_b[0] = HID_KEY_B;
//         tud_hid_keyboard_report(1, 0, keycode_b);
//         board_delay(5);
        
//         tud_hid_keyboard_report(1, 0, zeros);
//         board_delay(5);
//     }
//     else
//     {
//         tud_hid_keyboard_report(1, 0, NULL);
//     }
// }

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
// void hid_task(void)
// {
//     // Poll every 10ms
//     const uint32_t interval_ms = 10;
//     static uint32_t start_ms = 0;
//     if (board_millis() - start_ms < interval_ms)
//         return; // not enough time
//     start_ms += interval_ms;

//     uint32_t const btn = board_button_read();

//     // Remote wakeup
//     if (tud_suspended() && btn)
//     {
//         // Wake up host if we are in suspend mode
//         // and REMOTE_WAKEUP feature is enabled by host
//         tud_remote_wakeup();
//     }
//     else
//     {
//         // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
//         send_hid_report(REPORT_ID_KEYBOARD, btn);
//     }
// }

void send_keycodes(char* keys) {
    if (send_buffer_idx != 0) return;

    for(int i = 0; i < 64; i++) {
        if (keys[i] == 0xff) break;
        send_buffer[i] = keys[i];
    }
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

    if (send_buffer_idx < 64 && send_buffer[send_buffer_idx] != 0xff) {
        uint8_t keycode[6] = {0};
        keycode[0] = send_buffer[send_buffer_idx];
        tud_hid_keyboard_report(1, 0, keycode);
        send_buffer[send_buffer_idx] = 0xff;
        send_buffer_idx++;
    } else {
        send_buffer_idx = 0;
    }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    // (void)instance;

    // if (report_type == HID_REPORT_TYPE_OUTPUT)
    // {
    //     // Set keyboard LED e.g Capslock, Numlock etc...
    //     if (report_id == REPORT_ID_KEYBOARD)
    //     {
    //         // bufsize should be (at least) 1
    //         if (bufsize < 1)
    //             return;

    //         uint8_t const kbd_leds = buffer[0];

    //         if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
    //         {
    //             // Capslock On: disable blink, turn led on
    //             blink_interval_ms = 0;
    //             board_led_write(true);
    //         }
    //         else
    //         {
    //             // Caplocks Off: back to normal blink
    //             board_led_write(false);
    //             blink_interval_ms = BLINK_MOUNTED;
    //         }
    //     }
    // }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    // static uint32_t start_ms = 0;
    static bool led_state = 0;

    // // blink is disabled
    // if (!blink_interval_ms)
    //     return;

    // // Blink every interval ms
    // if (board_millis() - start_ms < blink_interval_ms)
    //     return; // not enough time
    // start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}
