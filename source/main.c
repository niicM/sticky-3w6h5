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

// #define MATRIX_ROW_PINS_L { GP7, GP8, GP9, GP10}
// #define MATRIX_COL_PINS_L { GP11, GP12, GP13, GP14, GP15 }

// #define MATRIX_ROW_PINS_R { P10, P11, P12, P05 }
// #define MATRIX_COL_PINS_R { P06, P13, P14, P01, P00 }

#define ROWS 4
#define MATRIX_ROW_PINS_L { 7, 8, 9, 10 }
#define COLS 5
#define MATRIX_COL_PINS_L { 11, 12, 13, 14, 15 }


void setup() {
    uint matrix_row_pins_l[ROWS] = MATRIX_ROW_PINS_L;
    uint matrix_col_pins_l[COLS] = MATRIX_COL_PINS_L;

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

void scan_l(bool grid[ROWS][COLS]) {
// void scan_l() {

    send_string(".");
    uint matrix_row_pins_l[ROWS] = MATRIX_ROW_PINS_L;
    uint matrix_col_pins_l[COLS] = MATRIX_COL_PINS_L;

    for (int row = 0; row < ROWS; row++) {  
        uint gpio_row = matrix_row_pins_l[row];
        gpio_put(gpio_row, 0);
        sleep_us(1);  // Give a microsecond to wait for the output to take effect
        for (int col = 0; col < COLS; col++) {
            uint gpio_col = matrix_col_pins_l[col]; 
            bool not_pressed = gpio_get(gpio_col);
            
            grid[row][col] = !not_pressed;
        }
        gpio_put(gpio_row, 1);
    }

    char buff[256];
    for (int row = 0; row < ROWS; row++) {  
        for (int col = 0; col < COLS; col++) {
            if(grid[row][col]) {
                sprintf(buff, " (%02d %02d) ", row, col);
                send_string(buff);
            }
        }
    }
}


void send_keycodes_task() {
    const uint32_t interval_ms = 2000;
    static uint32_t start_ms = 0;
    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;

    bool grid[ROWS][COLS];
    scan_l(grid);

    led_toggle();
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
