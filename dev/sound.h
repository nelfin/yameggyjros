#ifndef SOUND_H
#define SOUND_H

/* Number of main loop cycles remaining to play sound */
static uint16_t tone_duration;

void enable_sound(void);
void disable_sound(void);
void start_primitive_tone(uint16_t denom, uint16_t duration);

#endif
