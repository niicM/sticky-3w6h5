#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "pico/stdlib.h" 
#include "bsp/board.h"
#include "tusb.h"

// #include "layers.h"
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
    const uint32_t interval_ms = 800;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    void scan_l();

    // send_string("Hello World! ");
    // send_at_once();

    // int n_keys = 94; 
    // char mods[95] = {0};
    // char keys[95] = {0};
    // int i;
    // for (int i = 0; i < n_keys; i++) {
    //     mods[i] = out_kwerty[2 * i];
    //     keys[i] = out_kwerty[(2 * i) + 1];
    // }
    // // mods[i + 1] = 0xff;
    // keys[94] = 0xff;
    // send_keycodes(mods, keys);


    
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

// #define MATRIX_ROW_PINS_L { GP7, GP8, GP9, GP10}
// #define MATRIX_COL_PINS_L { GP11, GP12, GP13, GP14, GP15 }

// #define MATRIX_ROW_PINS_R { P10, P11, P12, P05 }
// #define MATRIX_COL_PINS_R { P06, P13, P14, P01, P00 }

#define ROWS 4
#define MATRIX_ROW_PINS_L { 7, 8, 9, 10}
#define COLS 5
#define MATRIX_COL_PINS_L { 11, 12, 13, 14, 15 }


void setup() {
    int matrix_row_pins_l[ROWS] = MATRIX_ROW_PINS_L;
    int matrix_col_pins_l[COLS] = MATRIX_COL_PINS_L;

    for (int row = 0; row < ROWS; row++) {  
        uint gpio = matrix_row_pins_l[row];
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_OUT);
        gpio_put(gpio, 1);
    }

    for (int col = 0; col < COLS; col++) {
        uint gpio = matrix_col_pins_l[col];
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
    }
}

// void scan_l(bool grid[18]) {
void scan_l() {
    int matrix_row_pins_l[ROWS] = MATRIX_ROW_PINS_L;
    int matrix_col_pins_l[COLS] = MATRIX_COL_PINS_L;
    char buff[16];

    for (int row = 0; row < ROWS; row++) {  
        uint gpio_row = matrix_row_pins_l[row];
        gpio_put(gpio_row, 0);

        for (int col = 0; col < COLS; col++) {
            uint gpio_col = matrix_col_pins_l[col]; 
            bool not_pressed = gpio_get(gpio_col);
            
            if (not_pressed) {

            } else {
                sprintf(buff, "(%02d %02d) ", row, col);
                send_string(buff);
            }
            
        }
        gpio_put(gpio_row, 1);
    }
}

int main(void) {

    board_init();
    tusb_init();
    stdio_init_all();
    setup();

    // #include "pico/multicore.h"
    // multicore_launch_core1(core1_main);
    
    while (1)
    {
        tud_task(); // tinyusb device task
        send_keycodes_task();
    }
}
