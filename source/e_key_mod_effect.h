#pragma once

#include <stdint.h>
#include "e_press_to_effect.h"


bool up_k_m_effect(uint8_t mod[MAX_MODS], uint8_t key_n, struct effect* effect);

bool start_fat_match(uint8_t mod[MAX_MODS], uint8_t key_n);

bool finish_fat_match(
    uint8_t mod_key, uint8_t target_key, bool collected[N_KEYS], 
    struct effect* effect
);
