#include "dev/leds.c"
#include "dev/sound.c"
#include "dev/keys.c"

#include "lib/music.c"

/* At 120 BPM, one crochet = 500ms */
uint16_t tetris[] = {
    tB4,   500,
    tGs4,  250,
    tA4,   250,
    tB4,   250,
    tD5,   125,
    tC5,   125,
    tA4,   250,
    tGs4,  250,
    tE4,   500,
    tE4,   250,
    tA4,   250,
    tC5,   500,
    tB4,   250,
    tA4,   250,
    tGs4,  250,
    tE4,   250,
    tGs4,  250,
    tA4,   250,
    tB4,   500,
    tC5,   500,
    tA4,   500,
    tE4,   500,
    tE4,   1000,
    1,     250,
    tF4,   500,
    tA4,   250,
    tC5,   250,
    tC5,   125,
    tC5,   125,
    tB4,   250,
    tA4,   250,
    tG4,   750,
    tE4,   250,
    tG4,   250,
    tA4,   125,
    tG4,   125,
    tF4,   250,
    tE4,   250,
    tGs4,  250,
    tE4,   250,
    tGs4,  250,
    tA4,   250,
    tB4,   500,
    tC5,   500,
    tA4,   500,
    tE4,   500,
    tE4,   500
};

#define SONG (sizeof(tetris) / sizeof(uint16_t))

int main(void) {
    uint16_t i;

    initialise_screen();
    initialise_keys();
    enable_sound();

    for (;;) {
        if (get_next_key() == B_LEFT) {
            for (i = 0; i < SONG; i += 2) {
                play_tone(tetris[i], (float) tetris[i+1]);
                while (is_sound_playing());
            }
        }
    }

    return 0;
}
