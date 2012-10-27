#include "dev/leds.c"

#include "lib/delay.c"
#include "lib/colours.h"

static inline void fill_screen(uint8_t colour) {
    uint8_t i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            rgb_screen[i][j] = (pixel_t) colour;
        }
    }
}

static inline void fill_column(uint8_t col, uint8_t colour) {
    uint8_t j;
    for (j = 0; j < 8; j++) {
        rgb_screen[col][j] = (pixel_t) colour;
    }
}

#define DELAY 50
#define WASH_SCREEN(colour) \
    for (col = 0; col < 8; col++) { \
        fill_column(col, colour); \
        for (i = 0; i < DELAY; i++) { \
            delay_us(20); \
        } \
    }

int main(void) {
    uint8_t lights = 0x1u;
    uint8_t col;
    uint8_t i;

    initialise_screen();

    for (;;) {
        WASH_SCREEN(cff0000);
        WASH_SCREEN(cffff00);
        WASH_SCREEN(c00ff00);
        WASH_SCREEN(c00ffff);
        WASH_SCREEN(c0000ff);
        WASH_SCREEN(cff00ff);
        status_lights = lights;
        lights = (lights << 1) | (lights >> 7);

        WASH_SCREEN(c550000);
        WASH_SCREEN(c555500);
        WASH_SCREEN(c005500);
        WASH_SCREEN(c005555);
        WASH_SCREEN(c000055);
        WASH_SCREEN(c550055);
        status_lights = lights;
        lights = (lights << 1) | (lights >> 7);
    }

    return 0;
}
