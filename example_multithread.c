#include "lib/colours.h"
/* Include serial code */
#include "dev/serial.c"
/* Include screen code */
#include "dev/leds.c"
/* Include multithreading code */
#include "dev/multithread.c"

inline void delay(uint16_t ms) {
   uint16_t i,j;
   uint16_t loop = F_CPU / 17000;  // this is approximate and needs calabration.
   for (i=0;i<ms;i++) {
      for (j=0;j<loop;j++);
   }
}

void fill_screen(uint8_t colour) {
    uint8_t i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            rgb_screen[i][j] = (pixel_t) colour;
        }
    }
}

static inline void set_column(uint8_t col, uint8_t colour, int height) {
    uint8_t j;
    for (j = 0; j <= height; j++) {
        rgb_screen[col][j] = (pixel_t) colour;
    }
    for(; j < 8; j++) {
        rgb_screen[col][j] = (pixel_t) (uint8_t) c000000;
    }
}

void thread1(void){
    int x;
    for (;;) {
        for(x = 0; x < 8; x++){
            delay(20);
            rgb_screen[0][x] = (pixel_t) (uint8_t) cff0000;
        }
        for(x = 0; x < 8; x++){
            delay(20);
            rgb_screen[0][x] = (pixel_t) (uint8_t) c000000;
        }
    }
}

void thread2(void){
    int x;
    for (;;) {
        for(x = 0; x < 8; x++){
            delay(20);
            rgb_screen[7][x] = (pixel_t) (uint8_t) cffff00;
        }
        for(x = 0; x < 8; x++){
            delay(20);
            rgb_screen[7][x] = (pixel_t) (uint8_t) c000000;
        }
    }
}

int main(void) {
    initialise_screen();
    initialise_serial();
    init_multithread();

    execute_parallel(thread1, thread2);

    return 0;
}
