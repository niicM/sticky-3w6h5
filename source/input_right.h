#pragma once

#include <stdbool.h>

#define ROWS 4
#define COLS 5

void setup_expander();
void scan_r(bool grid[ROWS][COLS]);
