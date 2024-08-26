#pragma once

#include <stdint.h> 
#include <stdbool.h>
#include "e_effects.h"


#define MAX_MODS 4
#define N_KEYS 36

// for example: a base key, ctrl, shift, alt, winkey...
#define MAX_COLLECTED 8

// Keys go from 0 to 35, so we need a value to indicate key absence.
#define NO_KEY 0xff

// The same is true for the layers.
#define NO_LAYER 0xff


struct press_to_effect {

    // dynamic: Changes with key presses and releasesceeuexii
    bool currdown[N_KEYS];
    bool curr_affected[N_KEYS][N_KEYS];
    
    // Once a key has been used to modify another, it can't do an action of it's 
    // own.
    bool cancelled[N_KEYS];


    // If activated, the key-up of that key will need to finish the effect.
    uint8_t waiting_for_release;
    
    // In practice, x in (ctrl + x), (alt + x) ect.   
    uint8_t target_key;

    // In waiting_for_release mode keypresses are collected for the combo.
    // For example one 
    bool collected[N_KEYS];
};


bool key_down(struct press_to_effect* pte, struct effect* effect, uint8_t key);

bool key_up(struct press_to_effect* pte, struct effect* effect, uint8_t key);

void init_press_to_effect(struct press_to_effect* pte);
