
#include "pico/stdlib.h" 
#include "bsp/board.h"

#include "usb_hid_out_fn.h"
#include "input_left.h"

#define MATRIX_ROW_PINS_L { 7, 8, 9, 10 }
#define MATRIX_COL_PINS_L { 11, 12, 13, 14, 15 }


void setup_gpio() {
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

    uint matrix_row_pins_l[ROWS] = MATRIX_ROW_PINS_L;
    uint matrix_col_pins_l[COLS] = MATRIX_COL_PINS_L;

    for (int row = 0; row < ROWS; row++) {  
        uint gpio_row = matrix_row_pins_l[row];
        gpio_put(gpio_row, 0);
        
        // Give a microsecond to wait for the output to take effect
        sleep_us(1); 
        
        for (int col = 0; col < COLS; col++) {
            uint gpio_col = matrix_col_pins_l[col]; 
            bool not_pressed = gpio_get(gpio_col);
            
            grid[row][col] = !not_pressed;
        }
        gpio_put(gpio_row, 1);
    }

    // char buff[256];
    // for (int row = 0; row < ROWS; row++) {  
    //     for (int col = 0; col < COLS; col++) {
    //         if(grid[row][col]) {
    //             sprintf(buff, " l(%02d %02d) ", row, col);
    //             send_string(buff);
    //         }
    //     }
    // }
}