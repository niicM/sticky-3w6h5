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

#define N_KEYS 36

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

#define KEY_F1 0x3a // Keyboard F1
#define KEY_F2 0x3b // Keyboard F2
#define KEY_F3 0x3c // Keyboard F3
#define KEY_F4 0x3d // Keyboard F4
#define KEY_F5 0x3e // Keyboard F5
#define KEY_F6 0x3f // Keyboard F6
#define KEY_F7 0x40 // Keyboard F7
#define KEY_F8 0x41 // Keyboard F8
#define KEY_F9 0x42 // Keyboard F9
#define KEY_F10 0x43 // Keyboard F10
#define KEY_F11 0x44 // Keyboard F11
#define KEY_F12 0x45 // Keyboard F12


uint8_t f_key_codes[] = {
    0,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12
};

// in_layer_x[key] = 'n' I want to type key 'n' (maybe modified by ctrl or alt)
static const char const in_layer_base[N_KEYS] = {
    ',', '.', 'p', 'y', 'x',   'f', 'g', 'c', 'r', 'l',
    'a', 'o', 'e', 'u', 'i',   'd', 'h', 't', 'n', 's',
    '\'',';', 'q', 'j', 'k',   'b', 'm', 'w', 'v', 'z',
    OOO, OOO, OOO, OOO, OOO, OOO
};

static const char const in_layer_base_caps[N_KEYS] = {
    '<', '>', 'P', 'Y', 'X',   'F', 'G', 'C', 'R', 'L',
    'A', 'O', 'E', 'U', 'I',   'D', 'H', 'T', 'N', 'S',
    '"', ':', 'Q', 'J', 'K',   'B', 'M', 'W', 'V', 'Z',
    OOO, OOO, OOO, OOO, OOO, OOO
};

static const char const in_layer_nums[NO_KEY] = {
    OOO, '&', '*', '?', OOO,   OOO, '7', '8', '9', OOO,
    OOO, '$', '%', '^', OOO,   OOO, '4', '5', '6', '0',
    OOO, '!', '@', '#', OOO,   OOO, '1', '2', '3', OOO, 
    OOO, OOO, OOO, OOO, OOO, OOO
};

static const char const in_layer_msim[N_KEYS] = {
    '+', '/', '(', ')', '~',   OOO, OOO, OOO, OOO, OOO,
    '-', '\\','[', ']', '`',   OOO, OOO, OOO, OOO, OOO,
    '_', '|', '{', '}', '=',   OOO ,OOO, OOO, OOO, OOO, 
    OOO, OOO, OOO, OOO, OOO, OOO
};

static const int8_t const in_layer_arrows[N_KEYS] = {
    OOO, OOO, OOO, OOO, OOO,   OOO,          OOO,      KEY_UP,   KEY_PAGEUP,   OOO,
    OOO, OOO, OOO, OOO, OOO,   KEY_CAPSLOCK, KEY_LEFT, KEY_DOWN, KEY_RIGHT,    OOO,
    OOO ,OOO, OOO, OOO, OOO,   OOO,          OOO,      OOO,      KEY_PAGEDOWN, OOO,
    OOO, OOO, OOO, OOO, OOO, OOO
};

static const int8_t const in_layer_fkeys[N_KEYS] = {
    OOO, OOO, OOO, OOO, OOO,   0x43, 0x40, 0x41, 0x42, OOO,
    OOO, OOO, OOO, OOO, OOO,   0x44, 0x3d, 0x3e, 0x3f, OOO,
    OOO ,OOO, OOO, OOO, OOO,   0x45, 0x3a, 0x3b, 0x3c, OOO,
    OOO, OOO, OOO, OOO, OOO, OOO
};

static const int8_t const in_layer_thumbs[] = {  // Meant to be key holds
    KEY_TAB, OOO, KEY_ENTER, KEY_BACKSPACE, OOO, KEY_DELETE
};

static const int8_t const in_layer_thumbs_type[] = {  // Meant to be key holds
    OOO, KEY_ESC, OOO, OOO, KEY_SPACE, OOO
};

static const uint8_t const in_layer_int[N_KEYS] = {
    0xff, 0x07, 0x08, 0x09, 0x0a,   0x0a, 0x07, 0x08, 0x09, 0xff, 
    0xff, 0x04, 0x05, 0x06, 0x0b,   0x0b, 0x04, 0x05, 0x06, 0x00, 
    0xff, 0x01, 0x02, 0x03, 0x0c,   0x0c, 0x01, 0x02, 0x03, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

//   00 01 02 03 04   05 06 07 08 09 
//   10 11 12 13 14   15 16 17 18 19 
//   20 21 22 23 24   25 26 27 28 29 
//         30 31 32   33 34 35

//   __ __ fp ff np   np __ __ __ __ 
//   cc __ mm 11 __   __ 11 mm __ cc
//   cp __ mp 1p __   __ 1p mp __ cp 
//         __ __ __   __ __ __ 


#define L_CAPS 10
#define L_NUMS 13
// #define L_MORE 12
#define ARROWS 31
#define ARROWS_SHIFT 13

#define R_CAPS 19
#define R_NUMS 16
#define R_MORE 17

// PLUS means that they are fat shortcuts
#define L_NORMAL_PLUS 4
#define L_CAPS_PLUS 20
#define L_NUMS_PLUS 23
#define L_MORE_PLUS 22

#define R_NORMAL_PLUS 5
#define R_CAPS_PLUS 29
#define R_NUMS_PLUS 26
#define R_MORE_PLUS 27

// This are for non-fat shortcuts for added convenience 
#define L_SHORTCUTS 14
#define R_SHORTCUTS 15

#define FUN 3
#define L_FUN_PLUS 2

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

    // After this only combos of one modifier
    if (m1 != NO_KEY) { return false; }

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_base[key_n];
    }

    // Shift
    else if (m0 == R_CAPS) {
        uint8_t k = in_layer_base_caps[key_n];
        ef.payload = k;
    }
    
    // Numbers (symbols)
    else if (m0 == R_NUMS) {
        uint8_t k = in_layer_nums[key_n];
        ef.payload = k;
    }
    
    // More (symbols)
    else if (m0 == R_MORE) {
        uint8_t k = in_layer_msim[key_n];
        ef.payload = k;
    }
    
    else if (m0 == R_SHORTCUTS) {
        uint8_t k = in_layer_base[key_n];
        ef.payload = k;
        ef.ctrl_alt = CTRL;
    }

    else {
        return false;
    }

    if (ef.payload == OOO) { return false; } 

    *effect = ef;
    return true;
}


bool k_m_effect_right(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1];

    printf("(up right: %d %d) ", m0, m1);

    struct effect ef = {ASCII_TYPE, NO_KEY, 0};


    // After this only combos of one modifier
    if (m1 != NO_KEY) { return false; }

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_base[key_n];
    } 

    // Shift
    else if (m0 == L_CAPS) {
        ef.payload = in_layer_base_caps[key_n];
    }

    // Numbmers
    else if (m0 == L_NUMS) {
        uint8_t k = in_layer_nums[key_n];
        ef.payload = k;
    }
    
    // Function keys
    else if (m0 == FUN) {
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

    else if (m0 == L_SHORTCUTS) {
        uint8_t k = in_layer_base[key_n];
        ef.payload = k;
        ef.ctrl_alt = CTRL;
    }

    else {
        return false;
    }

    if (ef.payload == OOO) { return false; } 

    *effect = ef;
    return true;
}


bool k_m_effect_thumbs(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    key_n -= 30;
    uint8_t m0 = mod[0]; 
    uint8_t m1 = mod[1];

    struct effect ef = {TYPE_KEY, NO_KEY, 0};

    // Base case
    if (m0 == NO_KEY) {
        ef.payload = in_layer_thumbs_type[key_n];
        ef.effect_type = TYPE_KEY;
    }
    
    else {
        return false; 
    }

    if (ef.payload == OOO) { return false; } 

    *effect = ef;
    return true;
}

/**
 * Returns false if there is no match.
 * Returns true and copies into the effect if there is a match
 */
bool up_k_m_effect(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect) {
    if (key_n >= 30) {
        return k_m_effect_thumbs(mod, key_n, effect);
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
    uint8_t m2 = mod[2];

    printf("(down: %d %d) ", m0, m1);

    struct effect ef = {PRESS_KEY, NO_KEY, 0};

    // Base case (thumbs)
    if (m0 == NO_KEY && key_n >= 30) {
        ef.payload = in_layer_thumbs[key_n - 30];
    } 
    
    // Arrows
    else if (m0 == ARROWS && m1 == NO_KEY) {
        if (key_n < 30) {
            ef.payload = in_layer_arrows[key_n];
        }
        else {
            ef.payload = in_layer_thumbs_type[key_n - 30];
        }
    }

    else if (m0 == ARROWS_SHIFT && m1 == ARROWS && m2 == NO_KEY) {
        ef.payload = in_layer_arrows[key_n];
        ef.ctrl_alt = SHIFT;
    }

    else {
        return false;
    }

    if (ef.payload == OOO) { return false; } 

    *effect = ef;
    return true;
}


#define FAT_LAYERS 9





#define ALLOW_LEFT 1 
#define ALLOW_RIGHT 2
#define ALLOW_ALL 0

const uint8_t const fat_layer_mods[FAT_LAYERS] = {
    L_NORMAL_PLUS,
    L_CAPS_PLUS,
    L_NUMS_PLUS,
    L_MORE_PLUS,
    
    R_NORMAL_PLUS,
    R_CAPS_PLUS,
    R_NUMS_PLUS,
    R_MORE_PLUS,
    
    L_FUN_PLUS
};

const char* const fat_layers[FAT_LAYERS] = {
    in_layer_base,
    in_layer_base_caps,
    in_layer_nums, 
    in_layer_arrows,

    in_layer_base,
    in_layer_base_caps,
    in_layer_nums, 
    in_layer_msim,

    in_layer_fkeys
};

const uint8_t const fat_layer_allow [FAT_LAYERS] = {
    ALLOW_RIGHT, ALLOW_RIGHT, ALLOW_RIGHT, ALLOW_RIGHT, 
    ALLOW_LEFT, ALLOW_LEFT, ALLOW_LEFT, ALLOW_LEFT,
    ALLOW_RIGHT, // function keys
};

const enum effect_type  const fat_layer_type [FAT_LAYERS] = {
    ASCII_TYPE,  // in_layer_base,
    ASCII_TYPE,  // in_layer_base_caps,
    ASCII_TYPE,  // in_layer_nums, 
    TYPE_KEY,  // in_layer_msim,

    ASCII_TYPE,  // in_layer_base,
    ASCII_TYPE,  // in_layer_base_caps,
    ASCII_TYPE,  // in_layer_nums, 
    ASCII_TYPE,  // in_layer_msim

    TYPE_KEY,  // in_layer_fkeys
};


// This are the patterns that can be completed with more keys afterwards
bool start_fat_match(uint8_t mods[MAX_MODS], uint8_t key) {

    uint8_t mod = mods[0]; 
    uint8_t mod_1 = mods[1]; 

    // No modifiers or too many (more than one)
    if (mod == NO_KEY || mod_1 != NO_KEY) {
        return false;
    }
    
    bool key_left = is_left(key);
    int allow = key_left * ALLOW_LEFT | !key_left * ALLOW_RIGHT;

    for (int i = 0; i < FAT_LAYERS; i++) {
        if (fat_layer_mods[i] == mod && fat_layer_allow[i] == allow) { 
            return true;
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
    uint8_t key = NO_KEY; 
    uint8_t type = ASCII_TYPE;

    bool key_left = is_left(target_key);
    int allow = key_left * ALLOW_LEFT | !key_left * ALLOW_RIGHT;

    for (int i = 0; i < FAT_LAYERS; i++) {
        bool is_allowed = 
           fat_layer_allow[i] == allow || fat_layer_allow[i] == ALLOW_ALL;

        if (is_allowed && fat_layer_mods[i] == mod_key) {
            key = fat_layers[i][target_key];
            type = fat_layer_type[i];

            // printf("match: k=%x t=%x l=%x\n", key, type, i);

            // The layers are not full in general.
            // If you type in a "hole" there is no effect.
            if (key == OOO) {
                *effect = no_effect;
                return false;
            }

            break;
        }
    }

    
    bool mod_alt;
    bool mod_ctr_and_alt;
    bool mod_win;

    if (key_left) {           
        mod_alt          = collected[13];
        mod_ctr_and_alt  = collected[12];
        mod_win          = collected[11];
    }
    else {
        mod_alt          = collected[16];
        mod_ctr_and_alt  = collected[17];
        mod_win          = collected[18];
    }

    uint8_t combinations[] = {
        CTRL,
        WIN,
        CTRL | ALT,
        WIN | CTRL,
        ALT,
        WIN | ALT,
        CTRL | ALT,
        WIN | CTRL | ALT,
    };

    uint8_t combination_idx =
        mod_alt * 0b100 | mod_ctr_and_alt * 0b010 | mod_win * 0b001;
    
    uint8_t modifier_keys = combinations[combination_idx];

    effect->ctrl_alt = modifier_keys; 
    effect->payload = key;
    effect->effect_type = type;
    return true;
}
