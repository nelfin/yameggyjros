#ifndef SOUND_H
#define SOUND_H

#include <avr/io.h>

#define PIEZO       PB1

// Piezo data register
#define PIEZO_DR    PORTB
// Piezo data direction register
#define PIEZO_DDR   DDRB

#define PIEZO_ON    (1 << PIEZO)
#define PIEZO_OFF   (~(1 << PIEZO))

void enable_piezo();
void disable_piezo();
void piezo_on();
void piezo_off();
void piezo_toggle();

#endif
