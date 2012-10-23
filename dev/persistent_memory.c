#include <avr/eeprom.h>
#define EEPROM_SIZE 1024-8*8 //save 64 bytes for screenshots
#define SCREENSHOT_LOCATION EEPROM_SIZE
uint8_t persistent_memory[EEPROM_SIZE]; //For writing and reading to

void write_byte(int address, uint8_t byte){
     eeprom_write_byte((uint8_t *) address, byte);
}

uint8_t read_byte(int i){
     return eeprom_read_byte((uint8_t *) i);
}

void save_persistent_memory(){
    int i = 0;
    for(i=0;i<EEPROM_SIZE;i++){
        write_byte(i,persistent_memory[i]);
    }
}

void load_persistent_memory(){
    int i = 0;
    for(i=0;i<EEPROM_SIZE;i++){
        persistent_memory[i] = read_byte(i);
    }
}

//You don't need to call this, so long as you call load_persistent_memory(); before you want to read from EEPROM
void initialise_persistent_memory(){
    load_persistent_memory();
}

void take_screenshot(){
    int x,y;
    for(x = 0; x < 8; x++){
        for(y = 0; y < 8; y++){
            persistent_memory[EEPROM_SIZE + x + y*8] = (uint8_t) rgb_screen[x][y].colour;
        }
    }
    save_persistent_memory();
}

void load_screenshot(){
    int x,y;
    for(x = 0; x < 8; x++){
        for(y = 0; y < 8; y++){
            rgb_screen[x][y] = (pixel_t) persistent_memory[EEPROM_SIZE + x + y*8]; 
        }
    }
}
