#include <avr/io.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "sound.h"

/* Set all channels dark */
static void inline set_dark(void) {
    PORTD |= 252u;
    PORTB |= 17u;
}

/* Busy-wait until SPI clears */
static void inline shift_out_wait(void) {
    while (!(SPSR & (1 << SPIF)));
}

void initialise_screen(void) {
    DDRD = 254u;
    PORTD = 254u;
    DDRB = 63u;
    PORTB = 255u;

    set_dark();

    /* Prepping Timer 2 (8-bit resolution) for screen refresh */

    /* Disable interrupts */
    cli();
    /* Clear timer on compare match */
    TCCR2A = 1 << WGM21;
    /* Pre-scale timer to 1/32 */
    TCCR2B = (1 << CS21) | (1 << CS20);
    /*
     * F_CPU >> 5 = pre-scaled clock
     */
    OCR2A = (F_CPU >> 5) / S_COLS / S_PASSES / S_FPS;
    /* Trigger interrupt on compare match */
    TIMSK2 = 1 << OCIE2A;
    /* Enable interrupts */
    sei();
}

/* Where are we in the drawing cycle? */
static uint8_t current_col = 0;
static pixel_t *current_col_p = (pixel_t *) rgb_screen;

static uint8_t cycle = 0;
static uint8_t even_pass = 1;
ISR(TIMER2_COMPA_vect) {
    pixel_t p;
    pixel_t *ptr;
    uint8_t bits;
    uint8_t temp_pb, temp_pd;

    if (++current_col >= S_COLS) {
        current_col = 0;
        current_col_p = (pixel_t *) rgb_screen;
        if (++cycle >= (1 << S_C)) {
            cycle = 0;
            even_pass ^= 1;
        }
    } else {
        current_col_p += 8;
    }

    /* Could there be some way to remove the screen/sound coupling? */
    if (tone_duration > 0) {
        tone_duration--;
        if (tone_duration == 0) {
            OCR1A = 0;
        }
    }

    set_dark();
    SPCR = (1 << SPE) | (1 << MSTR);

    if (even_pass) {
        /* Everyone lights up on even passes */
        SPDR = status_lights;

        /* Work backwards */
        ptr = current_col_p + 7;
        bits = 0;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 128;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 64;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 32;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 16;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 8;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 4;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 2;
        p = *ptr--;
        if (p.bits.r > cycle) bits |= 1;
        SPDR = bits;

        /* Interleave work, so don't busy wait for cycles */
        ptr = current_col_p + 7;
        bits = 0;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 128;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 64;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 32;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 16;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 8;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 4;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 2;
        p = *ptr--;
        if (p.bits.g > cycle) bits |= 1;
        SPDR = bits;

        ptr = current_col_p + 7;
        bits = 0;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 128;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 64;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 32;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 16;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 8;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 4;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 2;
        p = *ptr--;
        if (p.bits.b > cycle) bits |= 1;
        shift_out_wait();
        SPDR = bits;

    } else {
        /* Only vBRIGHT light up on odd passes */
        SPDR = status_lights;

        /* Work backwards */
        ptr = current_col_p + 7;
        bits = 0;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 128;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 64;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 32;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 16;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 8;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 4;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 2;
        p = *ptr--;
        if (p.bits.r > cycle && p.bits.v) bits |= 1;
        SPDR = bits;

        /* Interleave work, so don't busy wait for cycles */
        ptr = current_col_p + 7;
        bits = 0;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 128;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 64;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 32;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 16;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 8;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 4;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 2;
        p = *ptr--;
        if (p.bits.g > cycle && p.bits.v) bits |= 1;
        SPDR = bits;

        ptr = current_col_p + 7;
        bits = 0;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 128;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 64;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 32;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 16;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 8;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 4;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 2;
        p = *ptr--;
        if (p.bits.b > cycle && p.bits.v) bits |= 1;
        shift_out_wait();
        SPDR = bits;
    }

    /* Set col */
    temp_pb = 0;
    temp_pd = 0;
    switch (current_col) {
        case 0:
            temp_pb = 239u;
            break;
        case 1:
            temp_pb = 254u;
            break;
        default:
            temp_pd = ~(1 << (9 - current_col));
    }
    shift_out_wait();

    /* Latch pulse */
    PORTB |= 4u;
    if (current_col > 1) {
        PORTD &= temp_pd;
    } else {
        PORTB &= temp_pb;
    }
    PORTB &= 251u;

    SPCR = 0;
}
