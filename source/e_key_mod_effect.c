// #include <assert.h>
// #include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "e_effects.h"
#include "e_press_to_effect.h"


// To denote empty possitions in the helper layout arrays.
// Can be used as transparent, to search in other tables.
#define OOO 0x00


// in_layer_x[key] = 'n' I want to type key 'n' (maybe modified by ctrl or alt)
char in_layer_base[30] = {
    ',', '.', 'p', 'y', 'x',   'f', 'g', 'c', 'r', 'l',
    'a', 'o', 'e', 'u', 'i',   'd', 'h', 't', 'n', 's',
    '\'',';', 'q', 'j', 'k',   'b', 'm', 'w', 'v', 'z'
};

char in_layer_base_caps[30] = {
    '<', '>', 'P', 'Y', 'X',   'F', 'G', 'C', 'R', 'L',
    'A', 'O', 'E', 'U', 'I',   'D', 'H', 'T', 'N', 'S',
    '"', ':', 'Q', 'J', 'K',   'B', 'M', 'W', 'V', 'Z'
};

char in_layer_nums[30] = {
    OOO, '&', '*', '?', OOO,   OOO, '7', '8', '9', OOO,
    OOO, '$', '%', '^', OOO,   OOO, '4', '5', '6', '0',
    OOO, '!', '@', '#', OOO,   OOO, '1', '2', '3', OOO
};

char in_layer_msim[30] = {
    '+', '/', '(', ')', '~',   OOO, OOO, OOO, OOO, OOO,
    '-', '\\','[', ']', '`',   OOO, OOO, OOO, OOO, OOO,
    '_', '|', '{', '}', '=',   OOO ,OOO, OOO, OOO, OOO
};

uint8_t is_left(uint8_t key) {

    // This comparison is only needed if the last (six key) row is used in layers
    return (key < 33) && (key % 10 < 5);
    // return key % 10 < 5;
}


//   00 01 02 03 04   05 06 07 08 09 
//   10 11 12 13 14   15 16 17 18 19 
//   20 21 22 23 24   25 26 27 28 29 
//         30 31 32   33 34 35

//   __ __ __ __ __   __ __ __ __ __ 
//   __ __ __ __ __   __ __ __ __ __ 
//   __ __ __ __ __   __ __ __ __ __
//   __ __ __ __ __   __ __ __ __ __ 
//         __ __ __   __ __ __ 


bool k_m_effect_left(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1]; 
    uint8_t m2 = mod[2];
    uint8_t m3 = mod[3];

    struct effect ef = {ASCII_TYPE, NO_KEY, 0};

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_base[key_n];
    } 

    // Shift
    else if (m0 == 19) {
        if (m1 == NO_KEY) {
            uint8_t k = in_layer_base_caps[key_n];
            ef.payload = k;
        }
        else {
            return false;
        }
    }
    
    // Numbers (symbols)
    else if (m0 == 16) {
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
    else if (m0 == 26) {
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


/**
 * This function dessign is very manual and non-general by dessign.
 * If you want to keep the overal dessign but change the type of combos, you 
 * should modify these functions.
 * 
 * Trying to find a more general way of mapping keys and modifiers to effects 
 * can add more complexity than it's worth. This approach has the benefit of 
 * being fast too.
 */
bool k_m_effect_right(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1]; 
    uint8_t m2 = mod[2];
    uint8_t m3 = mod[3];

    struct effect ef = {ASCII_TYPE, NO_KEY, 0};

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_base[key_n];
    } 

    // Shift
    else if (m0 == 10) {
        if (m1 == NO_KEY) {
            ef.payload = in_layer_base_caps[key_n];
        } 
        else {
            return false;
        }
    }

    // Numbmers
    else if (m0 == 13) {
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

    // // Shorcuts
    // else if (11 <= m0 && m0 <= 13) {
    //     if (m0 == 13 && m1 == NO_KEY) {
    //         ef.ctrl_alt = CTRL;
    //         ef.payload = in_layer_base[key_n];
    //     }
    //     else if (m0 == 12 && m1 == 13 && m2 == NO_KEY) {
    //         ef.ctrl_alt = CTRL;
    //         ef.payload = in_layer_base_caps[key_n];
    //     }
    //     else if (m0 == 11 && m1 == 13 && m2 == NO_KEY) {
    //         ef.ctrl_alt = CTRL | ALT;
    //         ef.payload = in_layer_base[key_n];
    //     }
    //     else if (m0 == 11 && m1 == 12 && m2 == 13 && m3 == NO_KEY) {
    //         ef.ctrl_alt = CTRL | ALT;
    //         ef.payload = in_layer_base_caps[key_n];
    //     }
    //     else {
    //         return false; 
    //     }
    // } 

    // // Numbers
    // else if (21 <= m0 && m0 <= 23) {
    //     uint8_t key = in_layer_nums[key_n];
    //     if (key != OOO) {
    //         ef.payload = key;            
    //         if (m0 == 23 && m1 == NO_KEY) {}
    //         else if (m0 == 22 && m1 == 23 && m2 == NO_KEY) { 
    //             ef.ctrl_alt = CTRL;
    //         } 
    //         else if (m0 == 21 && m1 == 23 && m2 == NO_KEY) {
    //             ef.ctrl_alt = ALT;
    //         } 
    //         else if (m0 == 21 && m1 == 22 && m2 == 23 && m3 == NO_KEY) {
    //             ef.ctrl_alt = CTRL | ALT;
    //         }
    //         else {
    //             return false;
    //         }
    //     } else {
    //         return false;
    //     }
    // }

    else {
        return false;
    }

    *effect = ef;
    return true;
}

// https://github.com/y-salnikov/stm32_HID_keyboard/blob/master/usb_hid_keys.h
#define ENTER 0x28
#define SPACE 0x2c
#define BACKSPACE 0x2a
#define TAB 0x2b
#define ESC 0x29
#define CAPSLOCK 0x39

bool k_m_effect_special(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    
    effect->effect_type =  KEY_TYPE;
    uint8_t key_code;

    switch (key_n) {
    case 30:
        key_code = CAPSLOCK;
        break;
    case 31:
        key_code = BACKSPACE;
        break;
    case 32:
        key_code = ENTER;
        break;
    case 33:
        key_code = TAB;
        break;
    case 34:
        key_code = SPACE;
        break;
    case 35:
        key_code = ESC;
        break;
    
    default:
        break;
    }

    effect->payload = key_code;
    return true;
}

/**
 * Returns false if there is no match.
 * Returns true and copies into the effect if there is a match
 */
bool up_k_m_effect(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    if (key_n >= 30) {
        return k_m_effect_special(mod, key_n, effect);
    } 
    else if (is_left(key_n)) { 
        return k_m_effect_left(mod, key_n, effect);
    } 
    else {
        return k_m_effect_right(mod, key_n, effect);
    }
}

// This are the patterns that can be completed with more keys afterwards
bool start_fat_match(uint8_t mod[MAX_MODS], uint8_t key_n) {
    
    return mod[0] == 13 && mod[1] == NO_KEY; 

    // if (mod[0] == 13 && mod[1] == NO_KEY) {
    //     return true;
    // }
    // return false;
}
