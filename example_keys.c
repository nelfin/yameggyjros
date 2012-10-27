#include "dev/keys.c"
#include "dev/leds.c"
#include "dev/serial.c"

#include "lib/screen.c"
#include "lib/colours.h"

inline void delay(uint16_t ms) {
   uint16_t i,j;
   uint16_t loop = F_CPU / 17000;  // this is approximate and needs calabration.
   for (i=0;i<ms;i++) {
      for (j=0;j<loop;j++);
   }
}

int main(void) {
    initialise_screen();
    initialise_keys();
    initialise_serial();
    
    key_t cheat_code[10] = {D_UP,D_UP,D_DOWN,D_DOWN,D_LEFT,D_RIGHT,D_LEFT,D_RIGHT,B_LEFT,B_RIGHT};
    int i;
    for(i = 0; i < 10; i++){
        key_t next_key = get_next_key();
        serial_putdebug("key",next_key);
        if(next_key == cheat_code[i]){
            delay(100);
            continue;
        }else{
            serial_putstring("Incorrect\0");
            i=0;
        }
    }
    //Success
    while(1){
        fill_screen(c00ff00);
        delay(500);
        fill_screen(c0000ff);
        delay(500);
    }
}
