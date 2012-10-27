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
uint16_t const bgmusic[] = {
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

#define SONG (sizeof(bgmusic) / sizeof(uint16_t))

/*
 * Function prototypes, to make things easier/neater
 */
void pause_screen(void);
void spiral_screen(uint8_t colour, uint8_t slowdown);
void die_anim(uint8_t slowdown);
void play_breakmeggy(void);
void play_bgmusic(void);

int main(void) {
    initialise_multithreading();
    initialise_screen();
    initialise_keys();
    enable_sound();

    for (;;) {
        pause_screen();
        die_anim(150);
        execute_parallel(play_breakmeggy, play_bgmusic);
    }

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

void die_anim(uint8_t slowdown) {
    spiral_screen(c550000, slowdown);
    spiral_screen(c550055, slowdown);
    spiral_screen(c555555, slowdown);
    spiral_screen(c000000, slowdown);
}

/* breakmeggy constants */
#define TICKS 100
#define BALL_COLOUR (caa0000 & vDARK)

/* left = ~RIGHT; down = ~UP */
#define RIGHT 0x1u
#define UP    0x2u
#define STOP  0x4u

static void inline draw_paddle(uint8_t pos) {
    fill_row(0, c000000);
    rgb_screen[0+pos][0] = (pixel_t) (uint8_t) c005500;
    rgb_screen[1+pos][0] = (pixel_t) (uint8_t) c005500;
    rgb_screen[2+pos][0] = (pixel_t) (uint8_t) c005500;
}

static void inline draw_pixel(uint8_t x, uint8_t y, uint8_t colour) {
    rgb_screen[x][y] = (pixel_t) colour;
}

static void inline draw_blocks(uint8_t blocks[][4]) {
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (blocks[i][j]) {
                draw_pixel(2*i, j+4, cff5500);
                draw_pixel(2*i+1, j+4, cff5500);
            } else {
                draw_pixel(2*i, j+4, c000000);
                draw_pixel(2*i+1, j+4, c000000);
            }
        }
    }
}

void play_breakmeggy(void) {
    uint8_t i;
    uint8_t tick;
    uint8_t lives = 3;
    key_t keyp, lastp = 0;
    /* - 0 1 2 3 4 5 - */
    uint8_t pos = 2;
    uint8_t ball_x = 3, ball_y = 1;
    uint8_t ball_v = STOP;
    /* XXX: This could be packed if space becomes a serious issue */
    uint8_t blocks[4][4] = {
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    };

    /* 3 lives = 3 aux LEDs */
    status_lights = LED_2 | LED_1 | LED_0;
    draw_pixel(ball_x, ball_y, BALL_COLOUR);

    for (;;) {
        draw_paddle(pos);
        draw_blocks(blocks);
        draw_pixel(ball_x, ball_y, BALL_COLOUR);
        /*
         * Paddle movement and time delay
         */
        for (tick = 0; tick < TICKS; tick++) {
            keyp = get_key_down();
            if (keyp != lastp) {
                switch (keyp) {
                    case D_LEFT:
                        pos = (pos > 0) ? pos - 1 : 0;
                        if (ball_v == STOP) {
                            draw_pixel(ball_x, ball_y, c000000);
                            ball_x = (ball_x > 1) ? ball_x - 1 : 1;
                            draw_pixel(ball_x, ball_y, BALL_COLOUR);
                        }
                        draw_paddle(pos);
                        break;
                    case D_RIGHT:
                        pos = (pos < 5) ? pos + 1 : 5;
                        if (ball_v == STOP) {
                            draw_pixel(ball_x, ball_y, c000000);
                            ball_x = (ball_x < 6) ? ball_x + 1 : 6;
                            draw_pixel(ball_x, ball_y, BALL_COLOUR);
                        }
                        draw_paddle(pos);
                        break;
                    case B_RIGHT:
                        if (ball_v == STOP) {
                            ball_v = RIGHT | UP;
                        }
                        break;
                    default:
                        delay_us(2);
                        break;
                }
            }
            lastp = keyp;
            delay(20);
        }
        /*
         * Update world if ball is moving
         */
        if (ball_v != STOP) {
            /*
             * Clear old ball position
             */
            draw_pixel(ball_x, ball_y, c000000);
            /*
             * Apply standard ball velocity
             */
            ball_x = (ball_v & RIGHT) ? ball_x + 1 : ball_x - 1;
            ball_y = (ball_v & UP) ? ball_y + 1 : ball_y - 1;
            if (ball_x == 7) {
                ball_v &= ~RIGHT;
            }
            if (ball_x == 0) {
                ball_v |= RIGHT;
            }
            if (ball_y == 7) {
                ball_v &= ~UP;
            }
            /*
             * Check paddle hit
             */
            if (ball_y == 1 && (ball_x >= pos && ball_x <= pos + 2)) {
                ball_v |= UP;
            }
            /*
             * Collision detection
             */
            if (ball_y > 3) {
                /* Check block rows */
                for (i = 0; i < 4; i++) {
                    if (blocks[i][ball_y - 4] &&
                            (ball_x == 2*i || ball_x == 2*i+1)) {
                        blocks[i][ball_y - 4] = 0;
                        ball_v &= ~UP;
                        play_tone(tCs2, 100);
                        break;
                    }
                }
            }
            /*
             * Ball missed
             */
            if (ball_y == 0) {
                die_anim(75);
                if (--lives == 0) {
                    break;
                }
                /*
                 * Re-initialise game state
                 */
                pos = 2;
                ball_x = 3, ball_y = 1;
                ball_v = STOP;
                status_lights = LED_0;
                if (lives == 2) {
                    status_lights |= LED_1;
                }
            }
            draw_pixel(ball_x, ball_y, BALL_COLOUR);
        }
    }
    /*
     * Game over... ;_;
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
    status_lights = LED_NONE;
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
                delay(100);
            }
        }
    }
}

void play_bgmusic(void) {
    uint16_t i;
    for (;;) {
        for (i = 0; i < SONG; i += 2) {
            play_tone(bgmusic[i], (float) bgmusic[i+1]);
            while (is_sound_playing());
            delay(100);
        }
    }
}
