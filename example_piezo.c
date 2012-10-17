#include <avr/io.h>

#include "dev/leds.c"
#include "dev/sound.c"
#include "lib/music.c"

uint16_t tetris[] = {
    tB4,   1712,
    tGs4,  856,
    tA4,   856,
    tB4,   856,
    tD5,   428,
    tC5,   428,
    tA4,   856,
    tGs4,  856,
    tE4,   1712,
    tE4,   856,
    tA4,   856,
    tC5,   1712,
    tB4,   856,
    tA4,   856,
    tGs4,  856,
    tE4,   856,
    tGs4,  856,
    tA4,   856,
    tB4,   1712,
    tC5,   1712,
    tA4,   1712,
    tE4,   1712,
    tE4,   3428,
    1,     856,
    tF4,   1712,
    tA4,   856,
    tC5,   856,
    tC5,   428,
    tC5,   428,
    tB4,   856,
    tA4,   856,
    tG4,   2572,
    tE4,   856,
    tG4,   856,
    tA4,   428,
    tG4,   428,
    tF4,   856,
    tE4,   856,
    tGs4,  856,
    tE4,   856,
    tGs4,  856,
    tA4,   856,
    tB4,   1712,
    tC5,   1712,
    tA4,   1712,
    tE4,   1712,
    tE4,   1712
};

#define SONG (sizeof(tetris) / sizeof(uint16_t))

int main(void) {
    uint16_t i;
    uint8_t keyp;

    DDRC = 0;
    PORTC = 255U;

    initialise_screen();
    enable_sound();

    for (;;) {
        keyp = (~(PINC) & 0x01U);
        if (keyp) {
            for (i = 0; i < SONG; i += 2) {
                play_tone(tetris[i], (float) tetris[i+1]);
                while (is_sound_playing());
            }
        } else {
            for (i = 0; i < 10000; i++);
        }
    }

    return 0;
}
