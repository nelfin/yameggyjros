#include "music.h"

void play_tone(uint16_t tone, float ms) {
    uint16_t duration;
    duration = (uint16_t) ms / MS_PERIODS;
    start_primitive_tone(tone, duration);
}
