#include <avr/io.h>
#include "keys.h"

void initialise_keys(void) {
    PORTC = 255U;   // Set port c as pull ups
    DDRC = 0;       // Set port c as all inputs
}

int is_key_down(key_t key){
    return (~(PINC)&63) & key;
}

void wait_for_key(key_t key) {
    while (!is_key_down(key));
}

key_t get_next_key(void) {
    key_t keyp;
    while (!(keyp = (~(PINC)&63)));
    return keyp;
}
