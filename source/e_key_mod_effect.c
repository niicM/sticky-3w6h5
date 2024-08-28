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


#define L_CAPS   10
#define L_NUMS   13
#define L_MORE   12

#define R_CAPS   19
#define R_NUMS   16
#define R_MORE   17

#define L_NORMAL_PLUS 14
#define L_NUMS_PLUS   23
#define L_MORE_PLUS   22

#define R_NORMAL_PLUS 15
#define R_NUMS_PLUS   26
#define R_MORE_PLUS   27


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
    uint8_t m2 = mod[2];
    uint8_t m3 = mod[3];

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
    

    static const int8_t const keys[] = {CAPSLOCK, BACKSPACE, ENTER, TAB, SPACE, ESC};

    effect->effect_type =  KEY_TYPE;
    effect->payload = keys[key_n - 30];
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

const uint8_t const left_ms[] = {
    L_NORMAL_PLUS,
    L_NUMS_PLUS,
    L_MORE_PLUS,
};

const uint8_t const right_ms[] = {
    R_NORMAL_PLUS,
    R_NUMS_PLUS,
    R_MORE_PLUS
};


// This are the patterns that can be completed with more keys afterwards
bool start_fat_match(uint8_t mod[MAX_MODS], uint8_t key) {

    if (mod[0] == NO_KEY || mod[1] != NO_KEY) {
        return false;
    }

    // The mod and the key are in opposite halves
    if (is_left(key)) {
        for (int i = 0; i < 3; i++) {
            if (mod[0] == right_ms[i]) {
                return true;
            }
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            if (mod[0] == left_ms[i]) {
                return true;
            }
        }
    }

    return false;
}


bool finish_fat_match(
    uint8_t mod_key, uint8_t target_key, bool collected[N_KEYS], 
    struct effect* effect
) {
    char* in_layer;
    char* layers[3] = {
        in_layer_base, in_layer_nums, in_layer_msim
    };
    
    uint8_t key;
    for (int i = 0; i < 3; i++) {
        if (mod_key == left_ms[i] || mod_key == right_ms[i]) {
            key = layers[i][target_key];
            break;
        }
    }
    
    bool mod_alt;
    bool mod_ctrl;
    bool mod_win;
    bool mod_shift;

    if (is_left(target_key)) {            
        mod_alt   = collected[16];
        mod_ctrl  = collected[17];
        mod_win   = collected[18];
        mod_shift = collected[19];
    }
    else {            
        mod_alt   = collected[13];
        mod_ctrl  = collected[12];
        mod_win   = collected[11];
        mod_shift = collected[10];
    }

    // Ctrl is implied if there are no modifiers or only shift
    mod_ctrl = mod_ctrl ^ !(mod_alt || mod_win);

    uint8_t ctrl_alt = 
        mod_ctrl * CTRL | mod_shift * SHIFT | mod_alt * ALT | mod_win * WIN;

    // Consider using a default like ctrl, in this case
    if (ctrl_alt == 0) return false;

    effect->effect_type = ASCII_TYPE;
    effect->payload = key;
    effect->ctrl_alt = ctrl_alt;
    return true;
}
