#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pico/stdlib.h" 
#include "hardware/i2c.h"

#include "usb_hid_out_fn.h"
#include "input_right.h"

#define I2C_TIMEOUT 1000

// I2C address of PCA9555PW
#define PCA9555_ADDRESS 0x20

// PCA9555 Registers
#define PCA9555_INPUT_PORT_0    0x00
#define PCA9555_INPUT_PORT_1    0x01
#define PCA9555_OUTPUT_PORT_0   0x02
#define PCA9555_OUTPUT_PORT_1   0x03
#define PCA9555_POLARITY_INV_0  0x04
#define PCA9555_POLARITY_INV_1  0x05
#define PCA9555_CONFIG_0        0x06
#define PCA9555_CONFIG_1        0x07

void i2c_write_register(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    // int i2c_write_blocking(
    //    i2c_inst_t * i2c, uint8_t addr, const uint8_t * src, size_t len, 
    //    bool nostop)

    i2c_write_blocking(i2c0, PCA9555_ADDRESS, buffer, 2, false);
}

uint8_t i2c_read_register(uint8_t reg) {
    uint8_t value;
    i2c_write_blocking(i2c0, PCA9555_ADDRESS, &reg, 1, true);
    i2c_read_blocking(i2c0, PCA9555_ADDRESS, &value, 1, false);
    return value;
}

void setup_expander() {
    i2c_init(i2c0, 100 * 1000);  // 100 kHz
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    // Configure the PCA9555PW
    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0
    i2c_write_register(PCA9555_CONFIG_0, 0xFF);
    i2c_write_register(PCA9555_CONFIG_1, 0xF0);

    uint8_t conf[3] = {0x06, 0xFF, 0xF0};
    i2c_write_blocking(i2c0, PCA9555_ADDRESS, conf, 3, false);
}


void scan_r(bool grid[ROWS][COLS]) {

    // char buff[256];

    for (int row = 0; row < ROWS; row++) {

        // Example for row 2: 1111 1011
        uint8_t ports = ~(1 << row);
        i2c_write_register(PCA9555_OUTPUT_PORT_1, ports);
        uint8_t out = i2c_read_register(PCA9555_INPUT_PORT_0);
        out = ~out;
        // Example for colum 1: 0000 0010

        // sprintf(buff, "o(%02x %02x) ", ports, out);
        // send_string(buff);

        for (int col = 0; col < COLS; col++) {
            if(out & (1 << col)) {
                grid[row][col] = true;    
            } 
        }
    }

    // for (int row = 0; row < ROWS; row++) {  
    //     for (int col = 0; col < COLS; col++) {
    //         if(grid[row][col]) {
    //             sprintf(buff, " r(%02d %02d) ", row, col);
    //             send_string(buff);
    //         }
    //     }
    // }
}
