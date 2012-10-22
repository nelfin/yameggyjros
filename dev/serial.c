#include "serial.h"
#include <string.h>
#include <avr/io.h>

#define BAUD 9600

#include <util/setbaud.h>

/*
Simple serial output for the meggyjr
Eric McCreath 2012  - GPLv3
To connect to this ouput, execute
minicom -b 9600 -D /dev/ttyUSB0
On a separate terminal
*/
void initialise_serial(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0A &= ~(_BV(U2X0));

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void serial_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}

void serial_putstring(char s[]) {
    int len = strlen(s);
    int i;
    for(i=0;i<len;i++){
        serial_putchar(s[i]);
    }
}
