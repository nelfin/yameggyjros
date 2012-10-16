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

int main(void) {
    uint8_t lights = 0x1u;

    initialise_screen();

    while (1) {
        status_lights = lights;
        lights = (lights << 1) | (lights >> 7);
        fill_screen(caaffff);
        delay(200);
        fill_screen(c000000);
        delay(200);
        fill_screen(c550000);
        delay(200);
        fill_screen(caa0000);
        delay(200);
        fill_screen(cff0000);
        delay(200);
    }

    return 0;
}
