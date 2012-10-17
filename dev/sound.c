#include <avr/io.h>

#include "sound.h"

void enable_sound(void) {
    /*
     * COM1A1:0 = 1: toggle OC1A on compare match
     * WGM13:10 = 9: phase and freq. correct PWM
     * CS12:10  = 1: no clock pre-scaling
     */
    TCCR1A = (1 << COM1A0) | (1 << WGM10);
    TCCR1B = (1 << WGM13) | (1 << CS10);
    DDRB |= (1 << DDB1);
}

void disable_sound(void) {
    TCCR1A = 0;
    TCCR1B = 0;
    DDRB &= ~(1 << DDB1);
    PORTB |= (1 << PB1);
}

/*
 * uint16_t denom drives the PWM frequency. The resulting frequency is given
 * by f_PWM = F_CPU/(n*2*denom), where n is the prescaling factor (in the
 * current setup, this is 1, i.e. no prescaling).
 */
void start_primitive_tone(uint16_t denom, uint16_t duration) {
    OCR1A = denom;
    tone_duration = duration;
    enable_sound();
}

uint8_t is_sound_playing(void) {
    return OCR1A > 0;
}
