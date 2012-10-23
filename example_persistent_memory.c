#include "lib/colours.h"
/* Include serial code */
#include "dev/serial.c"
/* Include screen code */
#include "dev/leds.c"
/* Include persistent memory code */
#include "dev/persistent_memory.c"
#include <avr/io.h>

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

//Saves, and loads a screenshot on startup
int main(void) {
    initialise_screen();
    initialise_serial();
    initialise_persistent_memory();
    load_screenshot();
    int input_index=0;
    PORTC = 255U;   // Set port c as pull ups
    DDRC = 0;       // Set port c as all inputs
    
    while(1){
        uint8_t keyA = (~(PINC) & 0x01U); // A button. writes blue
        uint8_t keyB = (~(PINC) & 0x02U); // B button. writes red
        uint8_t keyC = (~(PINC) & 0x04U); // Up takes a screenshot
        uint8_t keyD = (~(PINC) & 0x08U); // Down clears the screen
        if(keyA){
            rgb_screen[input_index / 8][input_index % 8] = (pixel_t) (uint8_t) c0000ff;
            input_index++;
            input_index = input_index % (8*8);
            delay(100);
        }else if(keyB != 0){
            rgb_screen[input_index / 8][input_index % 8] = (pixel_t) (uint8_t) cff0000;
            input_index++;
            input_index = input_index % (8*8);
            delay(100);
        }else if(keyC != 0){
            take_screenshot();
            delay(100);
        }else if(keyD != 0){
            fill_screen(c000000);
            delay(100);
        }
    }
    return 0;
}
