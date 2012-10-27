#include "../dev/leds.c"

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

static inline void fill_row(uint8_t row, uint8_t colour) {
    uint8_t j;
    for (j = 0; j < 8; j++) {
        rgb_screen[j][row] = (pixel_t) colour;
    }
}


