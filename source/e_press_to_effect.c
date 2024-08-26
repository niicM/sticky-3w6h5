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
bool key_up(
    struct press_to_effect* pte, struct effect* effect, uint8_t key
) {
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

    // Design choice: attempting and faillig a combo also cancels the modifiers,
    // they won't trigger an effect when released.
    for (int m = 0; m < MAX_MODS; m++) {
        int mod_key = mods[m];
        if (mod_key != NO_KEY) {
            pte->cancelled[mod_key] = true;
        }
    }


    if (pte->waiting_for_release == NO_KEY) {
        if (start_fat_match(mods, key)) {
            pte->waiting_for_release = mods[0];
            pte->target_key = key;

            // This is simmilar to a match but it's internal. No real effect.
            return false;
        }
        else {
            return up_k_m_effect(mods, key, effect);
        }
    }
    else if (pte->waiting_for_release == key) {
        bool ret = fat_match(          // (ctrl + shit + a)
            pte->waiting_for_release,  // 13
            pte->target_key,           // 'a'
            pte->collected,            // 16
            effect
        );
        
        // No more in collected mode. Let's reset the tracking.
        memset(&pte->collected, false, N_KEYS * sizeof(bool));
        pte->target_key = NO_KEY;
        pte->waiting_for_release = NO_KEY;

        return ret;
    } 
    else {
        pte->collected[key] = !pte->collected[key];
    }
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

    *effect = no_effect;
    return false;
}


void init_press_to_effect(struct press_to_effect* pte) {
    // Don't assume any order in the struct to avoid errors.

    memset(&pte->currdown, false, N_KEYS * sizeof(bool));
    memset(&pte->curr_affected, false, N_KEYS * N_KEYS * sizeof(bool));
    
    // "canceled" doesn't really need to be initialed but it's good practice.
    memset(&pte->cancelled, 0, N_KEYS * sizeof(uint8_t));

    pte->waiting_for_release = NO_KEY;
    pte->target_key = NO_KEY;
    memset(&pte->collected, false, N_KEYS * sizeof(bool));
}