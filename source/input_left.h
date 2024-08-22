#pragma once

#include <stdbool.h>

#define ROWS 4
#define COLS 5

void setup_gpio();
void scan_l(bool grid[ROWS][COLS]);
