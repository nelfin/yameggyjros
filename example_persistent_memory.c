#include <avr/io.h>

#include "dev/serial.c"
#include "dev/leds.c"
#include "dev/persistent_memory.c"
#include "dev/keys.c"

#include "lib/screen.c"
#include "lib/colours.h"

inline void delay(uint16_t ms) {
   uint16_t i,j;
   uint16_t loop = F_CPU / 17000;  // this is approximate and needs calabration.
   for (i=0;i<ms;i++) {
      for (j=0;j<loop;j++);
   }
}

//Saves, and loads a screenshot on startup
int main(void) {
    initialise_keys();
    initialise_screen();
    initialise_serial();
    initialise_persistent_memory();
    load_screenshot();

    int input_index=0;

    while(1){
        uint8_t keyp = get_next_key();

        switch (keyp) {
            case D_DOWN:
                fill_screen(c000000);
                delay(100);
                break;
            case D_UP:
                take_screenshot();
                delay(100);
                break;
            case B_RIGHT:
                rgb_screen[input_index / 8][input_index % 8] = (pixel_t) (uint8_t) cff0000;
                input_index++;
                input_index = input_index % (8*8);
                delay(100);
                break;
            case B_LEFT:
                rgb_screen[input_index / 8][input_index % 8] = (pixel_t) (uint8_t) c0000ff;
                input_index++;
                input_index = input_index % (8*8);
                delay(100);
                break;
        }
    }

    return 0;
}
