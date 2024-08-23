// #include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "e_press_to_effect.h"
#include "e_key_mod_effect.h"
#include "e_effects.h"


/**
 * Returns wether there was a match,
 * Out: effect. NO_EFFECT can be the result of a match, or a result of no match
 */
bool key_up(struct press_to_effect* pte, struct effect* effect, uint8_t key) {
    printf("(k: %d) ", (int) key);
    
    pte->currdown[key] = false;
    // key no longer modifies any other keys
    for (int i = 0; i < N_KEYS; i++) {
        pte->curr_affected[i][key] = false;
    }
    if (pte->cancelled[key]) {
        *effect = no_effect;
        return false;
    }

    uint8_t mods[MAX_MODS];
    memset(mods, NO_KEY, MAX_MODS);
    int j = 0;
    for (int m_key = 0; m_key < N_KEYS && j < MAX_MODS; m_key++) {
        if (pte->curr_affected[key][m_key]) {
            printf("(m: %d) ", m_key);
            mods[j] = m_key;
            j++;
        }
    }
    
    bool matches = up_k_m_effect(mods, key, effect);
    if (!matches) {
        *effect = no_effect;

        // Design choice: attempting and faillig a combo cancels the modifiers
        for (int i = 0; i < MAX_MODS; i++) {
            if (mods[i] != NO_KEY) {
                pte->cancelled[mods[i]] = true;
            }
        } 
        
        return false;
    }

    for (int m = 0; m < MAX_MODS; m++) {
        int mod_key = mods[m];
        if (mod_key != NO_KEY) {
            pte->cancelled[mod_key] = true;
        }
    }
    return true;
}

bool key_down(struct press_to_effect* pte, struct effect* effect, uint8_t key) {
    
    // TODO Maybe here we can look for key "press" layer for things like ENTER, UP ect

    pte->currdown[key] = true;
    pte->cancelled[key] = false;
    // int total_down = 0;
    for (int i = 0; i < N_KEYS; i++) {
        pte->curr_affected[key][i] = pte->currdown[i];
        // total_down += (int) pte->currdown[i];
    }

    // This is a system to "cancel" all the current keys when you change your
    // mind.
    // if (total_down > MAX_MODS) {
    //     printf("canceling...");
    //     for (int i = 0; i < N_KEYS; i++)
    //     {
    //         pte->cancelled[i] = true;
    //     }
    // }

    *effect = no_effect;
    return false;
}


void init_press_to_effect(struct press_to_effect* pte) {
    // Don't assume any order in the struct to avoid errors.

    memset(&pte->currdown, 0, N_KEYS);
    memset(&pte->curr_affected, 0, N_KEYS * N_KEYS);
    pte->waiting_for_release = NO_KEY;
    
    // "canceled" doesn't really need to be initialed but it's good practice.
    memset(&pte->cancelled, 0, N_KEYS); 
}