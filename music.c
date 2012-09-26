/*
 * music.c - Play a melody on the ATmega
 */
/*
avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -Wall -o music.out music.c
avr-objcopy -O ihex -R .eeprom music.out music.hex
avrdude -b57600 -patmega328p -cstk500v1 -P/dev/ttyUSB0 -U flash:w:music.hex
*/


#include <avr/io.h>

#include "notes.h"

void delayMicroseconds(uint32_t us) {
   uint32_t i;
   for (i = 0; i < us; i++);
}

void initPiezo() {
    DDRB |= 1 << PB1;
}

void piezoHigh() {
    PORTB |= 1 << PB1;
}

void piezoLow() {
    PORTB &= ~(1 << PB1);
}

void initKeys() {
    DDRC = 0;
    PORTC = 255U;
}

// see notes.h for the meaning of these
uint16_t tetris[] = { b, gs,  a,  b,  D,  C,  a, gs,  e,  e,  a,  C,  b,  a, gs,  e, gs,  a,  b,  C,  a,  e,  e,
                      R,  f,  a,  C,  C,  C,  b,  a,  g,  e,  g,  a,  g,  f,  e, gs,  e, gs,  a,  b,  C,  a,  e,  e};
uint16_t beats[]  = {n4, n8, n8, n8,n16,n16, n8, n8, n4, n8, n8, n4, n8, n8, n8, n8, n8, n8, n4, n4, n4, n4, n2,
                     n8, n4, n8, n8,n16,n16, n8, n8, n3, n8, n8,n16,n16, n8, n8, n8, n8, n8, n8, n4, n4, n4, n4, n4};
uint32_t tempo = 5000; // a random number at the moment
uint32_t pause = 1000; // gap between the notes

uint16_t MAX_COUNT = sizeof(tetris) / sizeof(uint16_t);

uint32_t tone = 0;
uint32_t beat = 0;
uint32_t duration = 0;

void playTone() {
    uint32_t elapsed_time = 0;
    if (tone > 0) {
        while (elapsed_time < duration) {
            piezoHigh();
            delayMicroseconds(tone / 2);
            piezoLow();
            delayMicroseconds(tone / 2);
            elapsed_time += tone;
        }
    } else {
        delayMicroseconds(duration);
    }
}

int main(void) {
    uint16_t i;
    uint8_t keyp;

    initPiezo();
    initKeys();

    while (1) {
        keyp = (~(PINC) & 0x01U);
        if (keyp) {
            delayMicroseconds(20000);
            for (i = 0; i < MAX_COUNT; i++) {
                tone = tetris[i];
                beat = beats[i];
                duration = beat * tempo;
                playTone();
                delayMicroseconds(pause);
            }
        } else {
            delayMicroseconds(1000);
        }
    }

    return 0;
}
