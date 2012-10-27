/*
 * breakmeggy.c
 *
 * A simple Breakout-like game
 *
 * cc-by-sa 3.0
 */

#include "dev/multithread.c"
#include "dev/sound.c"
#include "dev/keys.c"

#include "lib/music.c"
#include "lib/screen.c"
#include "lib/delay.c"
#include "lib/colours.h"

/*
 * A bit of extra delay
 */
static void inline delay(uint8_t count) {
    uint8_t i;
    for (i = 0; i < count; i++) delay_us(40);
}

/*
 * Pause screen colours
 */
uint8_t const colour_cycle[16] = {
    c000055 & vDARK,
    c000055,
    c0000aa,
    c0000ff,
    c555555 & vDARK,
    c555555,
    caaaaaa,
    cffffff,
    c000055 & vDARK,
    c000055,
    c0000aa,
    c0000ff,
    c555555 & vDARK,
    c555555,
    caaaaaa,
    cffffff,
};

uint8_t const pulse[16] = {
    c000000,
    c000000,
    c000000,
    c550000 & vDARK,
    c550000,
    caa0000,
    caa0000,
    cff0000,
    cff0000,
    caa0000,
    caa0000,
    c550000,
    c550000 & vDARK,
    c000000,
    c000000,
    c000000,
};

/*
 * Game music
 * Note: at 120 BPM, one crochet = 500ms
 */
uint16_t const tetris[] = {
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

/*
 * Function prototypes, to make things easier/neater
 */
void pause_screen(void);
void spiral_screen(uint8_t colour, uint8_t slowdown);
void die_anim(void);
void play_breakmeggy(void);
void play_tetris(void);

int main(void) {
    initialise_screen();
    initialise_keys();
    enable_sound();

    for (;;) {
        pause_screen();
        die_anim();
        /*play_tetris();*/
    }

    /*init_multithread();*/
    /*execute_parallel(loading, play_tetris);*/

    return 0;
}

/*
 * Fill the screen with a colour by spiraling clockwise from the bottom left
 * corner
 */
void spiral_screen(uint8_t colour, uint8_t slowdown) {
    uint8_t i;
    uint8_t k = 0, l = 0;
    uint8_t m = 7, n = 7;
    while (k <= m && l <= n) {
        for (i = l; i <= n; i++) {
            rgb_screen[k][i] = (pixel_t) colour;
            delay(slowdown);
        }
        k++;
        for (i = k; i <= m; i++) {
            rgb_screen[i][n] = (pixel_t) colour;
            delay(slowdown);
        }
        n--;
        if (m >= k) {
            for (i = n; i >= l; i--) {
                rgb_screen[m][i] = (pixel_t) colour;
                delay(slowdown);
                /* Don't get caught out by underflow */
                if (i == 0) {
                    break;
                }
            }
            m--;
        }
        for (i = m; i >= k; i--) {
            rgb_screen[i][l] = (pixel_t) colour;
            delay(slowdown);
        }
        l++;
    }
}

void die_anim(void) {
    spiral_screen(c550000, 150);
    spiral_screen(c550055, 150);
    spiral_screen(c555555, 150);
    spiral_screen(c000000, 150);
}

#define RIGHT 0x1u
#define LEFT  0x2u
#define UP    0x4u
#define DOWN  0x8u

void play_breakmeggy(void) {
    uint8_t lives = 3;
    /* - 0 1 2 3 4 5 - */
    uint8_t pos = 2;
    uint8_t ball_x = 3, ball_y = 1;
    uint8_t ball_v = RIGHT | UP;
    for (;;) {

        /*
         * Ball missed
         */
        if (ball_y == 0) {
            if (--lives == 0) {
                break;
            }
            fill_screen(c550000);
            delay(200);

        }
    }
    /*
     * Game over...
     */
}

/*
 * (2,5) --> (5,5)
 *   ^         |
 *   |         |
 *   |         V
 * (2,2) <-- (5,2)
 */
void pause_screen(void) {
    uint8_t shift, c;
    key_t keyp = 0;
    while (!keyp) {
        for (shift = 16; shift > 0; --shift) {
            /* Glowing square */
            rgb_screen[3][3] = (pixel_t) pulse[shift & 0xf];
            rgb_screen[4][3] = (pixel_t) pulse[shift & 0xf];
            rgb_screen[3][4] = (pixel_t) pulse[shift & 0xf];
            rgb_screen[4][4] = (pixel_t) pulse[shift & 0xf];
            /* Top row */
            for (c = 0; c < 4; c++) {
                rgb_screen[2+c][5] = (pixel_t) colour_cycle[(c+shift) & 0xf];
            }
            /* Right column */
            for ( ; c < 8; c++) {
                rgb_screen[5][5-c+4] = (pixel_t) colour_cycle[(c+shift) & 0xf];
            }
            /* Bottom row */
            for ( ; c < 12; c++) {
                rgb_screen[5-c+8][2] = (pixel_t) colour_cycle[(c+shift) & 0xf];
            }
            /* Left column */
            for ( ; c < 16; c++) {
                rgb_screen[2][2+c-12] = (pixel_t) colour_cycle[(c+shift) & 0xf];
            }
            for (c = 0; (c < 10) && !keyp; c++) {
                keyp = get_key_down();
                delay(50);
            }
        }
    }
}

void play_tetris(void) {
    uint16_t i;
    for (;;) {
        for (i = 0; i < SONG; i += 2) {
            play_tone(tetris[i], (float) tetris[i+1]);
            while (is_sound_playing());
            delay(100);
        }
    }
}
