// #include <assert.h>
// #include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "e_effects.h"
#include "e_press_to_effect.h"

/**
 * This function dessign is very manual and non-general by dessign.
 * If you want to keep the overal dessign but change the type of combos, you 
 * should modify these functions.
 * 
 * Trying to find a more general way of mapping keys and modifiers to effects 
 * can add more complexity than it's worth. This approach has the benefit of 
 * being fast too.
 */

/**
 * This function dessign is very manual and non-general by dessign.
 * If you want to keep the overal dessign but change the type of combos, you 
 * should modify these functions.
 * 
 * Trying to find a more general way of mapping keys and modifiers to effects 
 * can add more complexity than it's worth. This approach has the benefit of 
 * being fast too.
 *//**
 * This function dessign is very manual and non-general by dessign.
 * If you want to keep the overal dessign but change the type of combos, you 
 * should modify these functions.
 * 
 * Trying to find a more general way of mapping keys and modifiers to effects 
 * can add more complexity than it's worth. This approach has the benefit of 
 * being fast too.
 */


// To denote empty possitions in the helper layout arrays.
// Can be used as transparent, to search in other tables.
// 0 doesn't have a meaning as a key code or as an ascii characer so it caan be 
// used in either.
#define OOO 0x00


// https://github.com/y-salnikov/stm32_HID_keyboard/blob/master/usb_hid_keys.h

#define KEY_ENTER 0x28
#define KEY_SPACE 0x2c
#define KEY_BACKSPACE 0x2a
#define KEY_DELETE 0x4c
#define KEY_TAB 0x2b
#define KEY_ESC 0x29

#define KEY_CAPSLOCK 0x39
// #define KEY_NUMLOCK 0x53

#define KEY_HOME 0x4a
#define KEY_END 0x4d

#define KEY_PAGEUP 0x4b
#define KEY_PAGEDOWN 0x4e

#define KEY_RIGHT 0x4f
#define KEY_LEFT 0x50
#define KEY_DOWN 0x51
#define KEY_UP 0x52

#define KEY_VOLUMEUP 0x80
#define KEY_VOLUMEDOWN 0x81
#define KEY_POWER 0x66

uint8_t f_key_codes[] = {
    0x00,
    0x3a, // Keyboard F1
    0x3b, // Keyboard F2
    0x3c, // Keyboard F3
    0x3d, // Keyboard F4
    0x3e, // Keyboard F5
    0x3f, // Keyboard F6
    0x40, // Keyboard F7
    0x41, // Keyboard F8
    0x42, // Keyboard F9
    0x43, // Keyboard F10
    0x44, // Keyboard F11
    0x45  // Keyboard F12
};

// in_layer_x[key] = 'n' I want to type key 'n' (maybe modified by ctrl or alt)
static const char const in_layer_base[30] = {
    ',', '.', 'p', 'y', 'x',   'f', 'g', 'c', 'r', 'l',
    'a', 'o', 'e', 'u', 'i',   'd', 'h', 't', 'n', 's',
    '\'',';', 'q', 'j', 'k',   'b', 'm', 'w', 'v', 'z'
};

static const char const in_layer_base_caps[30] = {
    '<', '>', 'P', 'Y', 'X',   'F', 'G', 'C', 'R', 'L',
    'A', 'O', 'E', 'U', 'I',   'D', 'H', 'T', 'N', 'S',
    '"', ':', 'Q', 'J', 'K',   'B', 'M', 'W', 'V', 'Z'
};

static const char const in_layer_nums[30] = {
    OOO, '&', '*', '?', OOO,   OOO, '7', '8', '9', OOO,
    OOO, '$', '%', '^', OOO,   OOO, '4', '5', '6', '0',
    OOO, '!', '@', '#', OOO,   OOO, '1', '2', '3', OOO
};

static const char const in_layer_msim[30] = {
    '+', '/', '(', ')', '~',   OOO, OOO, OOO, OOO, OOO,
    '-', '\\','[', ']', '`',   OOO, OOO, OOO, OOO, OOO,
    '_', '|', '{', '}', '=',   OOO ,OOO, OOO, OOO, OOO
};

static const int8_t const in_layer_arrows[30] = {
    OOO, OOO, OOO, OOO, OOO,   OOO, OOO,      KEY_UP,   KEY_PAGEUP,   OOO,
    OOO, OOO, OOO, OOO, OOO,   OOO, KEY_LEFT, KEY_DOWN, KEY_RIGHT,    KEY_CAPSLOCK,
    OOO ,OOO, OOO, OOO, OOO,   OOO, OOO,      OOO,      KEY_PAGEDOWN, OOO
};

static const int8_t const in_layer_thumbs[] = {  // Meant to be key holds
    KEY_TAB, KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_SPACE, KEY_DELETE
};

static const uint8_t const in_layer_int[30] = {
    0xff, 0x07, 0x08, 0x09, 0x10,   0x10, 0x07, 0x08, 0x09, 0xff, 
    0xff, 0x04, 0x05, 0x06, 0x11,   0x11, 0x04, 0x05, 0x06, 0x00, 
    0xff, 0x01, 0x02, 0x03, 0x12,   0x12, 0x01, 0x02, 0x03, 0xff
};

//   00 01 02 03 04   05 06 07 08 09 
//   10 11 12 13 14   15 16 17 18 19 
//   20 21 22 23 24   25 26 27 28 29 
//         30 31 32   33 34 35

//   __ __ fp ff __   __ __ __ __ __ 
//   cc __ mm 11 np   np 11 mm __ cc
//   cp __ mp 1p __   __ 1p mp __ cp 
//         __ __ __   __ __ __ 


#define L_CAPS 10
#define L_NUMS 13
#define L_MORE 12

#define R_CAPS 19
#define R_NUMS 16
#define R_MORE 17

#define L_NORMAL_PLUS 14
#define L_CAPS_PLUS 20
#define L_NUMS_PLUS 23
#define L_MORE_PLUS 22

#define R_NORMAL_PLUS 15
#define R_CAPS_PLUS 29
#define R_NUMS_PLUS 26
#define R_MORE_PLUS 27

#define FUN 3
#define FUN_PLUS 2

uint8_t is_left(uint8_t key) {

    // This comparison is only needed if the last (six key) row is used in layers
    return (key < 33) && (key % 10 < 5);
    // return key % 10 < 5;
}


bool k_m_effect_left(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1];
    printf("(up left: %d %d) ", m0, m1);

    struct effect ef = {ASCII_TYPE, NO_KEY, 0};

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_base[key_n];
    } 

    // Shift
    else if (m0 == R_CAPS) {
        if (m1 == NO_KEY) {
            uint8_t k = in_layer_base_caps[key_n];
            ef.payload = k;
        }
        else {
            return false;
        }
    }
    
    // Numbers (symbols)
    else if (m0 == R_NUMS) {
        if (m1 == NO_KEY) {
            uint8_t k = in_layer_nums[key_n];
            if (k != OOO) {
                ef.payload = k;
            }
            else {
                return false;
            }
        } 
        else {
            return false;
        }
    }
    
    // More (symbols)
    else if (m0 == R_MORE) {
        if (m1 == NO_KEY) {
            uint8_t k = in_layer_msim[key_n];
            if (k != OOO) {
                ef.payload = k;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    else {
        return false;
    }

    *effect = ef;
    return true;
}


bool k_m_effect_right(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1];

    printf("(up right: %d %d) ", m0, m1);

    struct effect ef = {ASCII_TYPE, NO_KEY, 0};

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_base[key_n];
    } 

    // Shift
    else if (m0 == L_CAPS) {
        if (m1 == NO_KEY) {
            ef.payload = in_layer_base_caps[key_n];
        } 
        else {
            return false;
        }
    }

    // Numbmers
    else if (m0 == L_NUMS) {
        if (m1 == NO_KEY) {
            uint8_t k = in_layer_nums[key_n];
            if (k != OOO) {
                ef.payload = k;
            }
            else {
                return false;
            }
        } 
        else {
            return false;
        }
    }
    
    // Function keys
    else if (m0 == FUN) {
        if (m1 == NO_KEY) { 
            printf("fun ");
            uint8_t n = in_layer_int[key_n];
            if (n < 1 || n > 12) {
                printf("out %d ", n);
                return false;
            }
            uint8_t f_code = f_key_codes[n];
            ef.payload = f_code;
            ef.effect_type = TYPE_KEY;
        }
        else {
            return false;
        }
    }

    else {
        return false;
    }

    *effect = ef;
    return true;
}


/**
 * Returns false if there is no match.
 * Returns true and copies into the effect if there is a match
 */
bool up_k_m_effect(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    if (key_n >= 30) {
        return false;
        // return k_m_effect_special(mod, key_n, effect);
    } 
    else if (is_left(key_n)) { 
        return k_m_effect_left(mod, key_n, effect);
    } 
    else {
        return k_m_effect_right(mod, key_n, effect);
    }
}


bool down_k_m_effect(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1]; 
    printf("(down: %d %d) ", m0, m1);

    struct effect ef = {PRESS_KEY, NO_KEY, 0};

    // Base case
    if (m0 == NO_KEY && key_n >= 30) {
        ef.payload = in_layer_thumbs[key_n - 30];
    } 
    
    // Arrows
    else if (m0 == L_MORE) {
        if (m1 == NO_KEY) {
            uint8_t k = in_layer_arrows[key_n];
            if (k != OOO) {
                ef.payload = k;
            }
            else {
                return false;
            }
        } 
        else {
            return false;
        }
    }

    else {
        return false;
    }

    *effect = ef;
    return true;
}

// Note the abscense of the upper-case layer, since it falls into the base layer 
// with shift modifier on.
#define FAT_LAYERS 4

const char* const fat_layers[FAT_LAYERS] = {
    in_layer_base,
    in_layer_base_caps,
    in_layer_nums, 
    in_layer_msim
};

const uint8_t const left_ms[FAT_LAYERS] = {
    L_NORMAL_PLUS,
    L_CAPS_PLUS,
    L_NUMS_PLUS,
    L_MORE_PLUS
};

const uint8_t const right_ms[FAT_LAYERS] = {
    R_NORMAL_PLUS,
    R_CAPS_PLUS,
    R_NUMS_PLUS,
    R_MORE_PLUS
};


// This are the patterns that can be completed with more keys afterwards
bool start_fat_match(uint8_t mod[MAX_MODS], uint8_t key) {

    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1]; 

    // No modifiers or too many (more than one)
    if (m0 == NO_KEY || m1 != NO_KEY) {
        return false;
    }

    // The mod and the key are in opposite halves
    if (is_left(key)) {
        for (int i = 0; i < FAT_LAYERS; i++) {
            if (m0 == right_ms[i]) {
                return true;
            }
        }
    }
    else {
        for (int i = 0; i < FAT_LAYERS; i++) {
            if (m0 == left_ms[i]) {
                return true;
            }
        }
    }

    return false;
}


bool finish_fat_match(
    uint8_t mod_key,  // The fist key you press down (determines the layer: letters, symbols...)
    uint8_t target_key,  // The second key (determines the actual key: a, b...)
    bool collected[N_KEYS],  // The subsequent keys (determines the modifiers: alt, shift...)
    struct effect* effect
) {
    char* in_layer;
    uint8_t key;

    bool is_target_left = is_left(target_key);

    for (int i = 0; i < 3; i++) {
        if (mod_key == left_ms[i] && !is_target_left) {
            key = fat_layers[i][target_key];
            break;
        }
        if (mod_key == right_ms[i] && is_target_left) {
            key = fat_layers[i][target_key];
            break;
        }

        // if (mod_key == left_ms[i] || mod_key == right_ms[i]) {
        //     key = fat_layers[i][target_key];
        //     break;
        // }
    }
    
    bool mod_alt;
    bool mod_ctr_and_alt;
    bool mod_win;

    if (is_target_left) {           
        mod_alt          = collected[13];
        mod_ctr_and_alt  = collected[12];
        mod_win          = collected[11];
    }
    else {
        mod_alt          = collected[16];
        mod_ctr_and_alt  = collected[17];
        mod_win          = collected[18];
    }

    uint8_t modifier_keys = 0;
    modifier_keys |= (mod_ctr_and_alt || (!mod_alt && !mod_win))  * CTRL;
    modifier_keys |= (mod_alt || mod_ctr_and_alt) * ALT;
    modifier_keys |= mod_win * WIN;
    
    effect->effect_type = ASCII_TYPE;
    effect->payload = key;
    effect->ctrl_alt = modifier_keys;
    return true;
}
