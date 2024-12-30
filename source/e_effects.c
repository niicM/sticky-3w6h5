#include <stdio.h>
#include "e_effects.h"

struct effect no_effect = {NO_EFFECT, 0, 0};
struct effect clear = {CLEAR, 0, 0};

char* not_recognized = "[not recognized]";

const char *key_names[] = {
    "volumeup",
    "volumedown",
    "power",
    "enter", 
    "space", 
    "backspace", 
    "delete", 
    "tab", 
    "esc", 
    "capslock", 
    "numlock", 
    "home", 
    "end", 
    "pageup", 
    "pagedown", 
    "right", 
    "left", 
    "down", 
    "up",
    "f1", 
    "f2", 
    "f3", 
    "f4", 
    "f5", 
    "f6", 
    "f7", 
    "f8", 
    "f9", 
    "f10", 
    "f11", 
    "f12"
};

const uint8_t key_values[] = {
    0x80,
    0x81,
    0x66,
    0x28, // enter
    0x2c, // space
    0x2a, // backspace
    0x4c, // delete
    0x2b, // tab
    0x29, // esc
    0x39, // capslock
    0x53, // numlock
    0x4a, // home
    0x4d, // end
    0x4b, // pageup
    0x4e, // pagedown
    0x4f, // right
    0x50, // left
    0x51, // down
    0x52, // up
    0x3a, // f1
    0x3b, // f2
    0x3c, // f3
    0x3d, // f4
    0x3e, // f5
    0x3f, // f6
    0x40, // f7
    0x41, // f8
    0x42, // f9
    0x43, // f10
    0x44, // f11
    0x45  // f12
};

#define NUM_KEYS (sizeof(key_values) / sizeof(key_values[0]))

const char* key_name(uint8_t key) {
    printf("k=%d\n", key);
    for (int i = 0; i < NUM_KEYS; i++) {
        if (key_values[i] == key) return key_names[i];
    }
    return not_recognized;
}

void print_effect(const struct effect* ef) {
    char buff[128];
    sprint_effect(ef, buff);
    printf("%s\n", buff);
}


void sprint_effect(const struct effect* ef, char buff[128]) {
    int advance = 0;
    
    if(ef->effect_type == NO_EFFECT) {
        advance = sprintf(buff, ".");
        buff += advance;
        return;
    }


    advance = sprintf(buff, " e(");
    buff += advance;
        
    if(ef->effect_type == PRESS_KEY || ef->effect_type == ASCII_DOWN) {
        advance = sprintf(buff, "d ");
        buff += advance;
    }
    if(ef->effect_type == TYPE_KEY || ef->effect_type == PRESS_KEY) {
        advance = sprintf(buff, "%s", key_name(ef->payload));
        buff += advance;
    }

    else if(ef->effect_type == ASCII_TYPE || ef->effect_type == ASCII_DOWN) {
        advance = sprintf(buff, "'%c'", ef->payload);
        buff += advance;
    }

    if(ef->effect_type == ASCII_TYPE || ef->effect_type == ASCII_DOWN 
        || ef->effect_type == TYPE_KEY || ef->effect_type == PRESS_KEY) {

        if(ef->ctrl_alt & CTRL) {
            advance = sprintf(buff, " ctrl");
            buff += advance;
        }
        if(ef->ctrl_alt & ALT) {
            advance = sprintf(buff, " alt");
            buff += advance;
        }
        if(ef->ctrl_alt & WIN) {
            advance = sprintf(buff, " win");
            buff += advance;
        }
    }

    advance = sprintf(buff, ")");
    buff += advance;
}