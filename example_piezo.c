#include "dev/sound.c"

void delay(uint16_t ms) {
   uint16_t i,j;
   uint16_t loop = F_CPU / 17000;  // this is approximate and needs calabration.
   for (i=0;i<ms;i++) {
      for (j=0;j<loop;j++);
   }
}

int main(void) {
    enable_sound();

    for (;;) {
        start_primitive_tone(10000, 500);
        delay(100);
        start_primitive_tone(16000, 500);
        delay(50);
        start_primitive_tone(12000, 500);
        delay(150);
        start_primitive_tone(14500, 500);
        delay(100);
        disable_sound();
        delay(1000);
    }

    return 0;
}
