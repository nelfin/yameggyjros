#include <avr/io.h>
#include "keys.h"

void initialise_keys(){
    PORTC = 255U;   // Set port c as pull ups
    DDRC = 0;       // Set port c as all inputs
}

int is_key_down(key_t key){
    return ((~(PINC)&63) & key) != 0;
}

void wait_for_key(key_t key){
    for(;;){
        if(is_key_down(key)){
            break;
        }
    }
}

key_t get_next_key(){
    for(;;){
        if((~(PINC)&63) != 0){
            return (key_t)(~(PINC)&63) ;
        }
    }
}
