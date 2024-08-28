#pragma once

#include <stdint.h>

#define CTRL  0x01
#define SHIFT 0x02
#define ALT   0x04
#define WIN   0x08

enum effect_type {

    NO_EFFECT,

    // Printable standard ascii chars (affected by layers e.g. Querty)
    ASCII_TYPE,
    ASCII_DOWN,

    // Other symbols not pressent in standard ascii (also affected by layers)
    EXTENDED_TYPE,
    EXTENDED_DOWN,

    // Refered to the actuall keycode (for keys like enter, tab...)
    KEY_TYPE,
    KEY_DOWN,

    // To release all keys that are being hold down, whithout sending a new key
    CLEAR,

    // Effects that change the state of the algorithm 
    // (e.g. change from Querty to Dvorak)
    INTERNAL    
};

struct effect {

    enum effect_type effect_type;
    
    // The meaning of payload depends on the type (ascii, hid-key, internal)
    uint8_t payload; 
    
    // ctrl (1), left alt (4), right alt (64) ...
    // Not used in clear or internal
    uint8_t ctrl_alt; 
};

extern struct effect no_effect;
extern struct effect clear;

void print_effect(struct effect* ef);