#include "sound.h"

void enable_piezo() {
    PIEZO_DDR |= PIEZO_ON;
}

void disable_piezo() {
    PIEZO_DDR &= PIEZO_OFF;
}

void piezo_on() {
    PIEZO_DR |= PIEZO_ON;
}

void piezo_off() {
    PIEZO_DR &= PIEZO_OFF;
}

void piezo_toggle() {
    PIEZO_DR ^= PIEZO_ON;
}
