#include <stdio.h>
#include "e_effects.h"

struct effect no_effect = {NO_EFFECT, 0, 0};
struct effect clear = {CLEAR, 0, 0};

void print_effect(struct effect* ef) {
    switch (ef->effect_type)
    {
    case NO_EFFECT:
        printf("No effect\n");
        break;

    case ASCII_TYPE:
        printf("Ascii type %c (%x)\n", ef->payload, ef->ctrl_alt);
        break;

    case TYPE_KEY:
        printf("Key type %x (%x)\n", ef->payload, ef->ctrl_alt);
        break;

    case PRESS_KEY:
        printf("Key hold %x (%x)\n", ef->payload, ef->ctrl_alt);
        break;

    default:
        printf("Other\n");
        break;
    }
}

void sprint_effect(struct effect* ef, char buff[128]) {
    
    int advance = 0;

    switch (ef->effect_type) {
    
    case ASCII_TYPE:

        // example result:
        //     e(ascii 'a' shift ctrl alt win)

        advance = sprintf(buff, " e(ascii '%c'", ef->payload);
        buff += advance;

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
        advance = sprintf(buff, ")");

        break;

    default:

        advance = sprintf(buff, "");
        buff += advance;
        break;
    }
}