#include "lib/colours.h"

/* Include screen code */
#include "dev/leds.c"

void delay(uint16_t ms) {
   uint16_t i,j;
   uint16_t loop = F_CPU / 17000;  // this is approximate and needs calabration.
   for (i=0;i<ms;i++) {
      for (j=0;j<loop;j++);
   }
}

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
int main(void) {
    uint8_t lights = 0x1u;
    uint8_t col;

    initialise_screen();

    while (1) {
        for (col = 0; col < 8; col++) {
            status_lights = lights;
            lights = (lights << 1) | (lights >> 7);
            fill_column(col, cff0000);
            delay(20);
        }
        for (col = 0; col < 8; col++) {
            status_lights = lights;
            lights = (lights << 1) | (lights >> 7);
            fill_column(col, cffff00);
            delay(20);
        }
        for (col = 0; col < 8; col++) {
            status_lights = lights;
            lights = (lights << 1) | (lights >> 7);
            fill_column(col, c00ff00);
            delay(20);
        }
        for (col = 0; col < 8; col++) {
            status_lights = lights;
            lights = (lights << 1) | (lights >> 7);
            fill_column(col, c00ffff);
            delay(20);
        }
        for (col = 0; col < 8; col++) {
            status_lights = lights;
            lights = (lights << 1) | (lights >> 7);
            fill_column(col, c0000ff);
            delay(20);
        }
        for (col = 0; col < 8; col++) {
            status_lights = lights;
            lights = (lights << 1) | (lights >> 7);
            fill_column(col, cff00ff);
            delay(20);
        }
    }

    return 0;
}
