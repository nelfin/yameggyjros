#ifndef LEDS_H
#define LEDS_H

/*
 * Auxilliary LEDs
 */
static uint8_t volatile status_lights;

#define LED_0    0x01u
#define LED_1    0x02u
#define LED_2    0x04u
#define LED_3    0x08u
#define LED_4    0x10u
#define LED_5    0x20u
#define LED_6    0x40u
#define LED_7    0x80u
#define LED_NONE 0x00u
#define LED_ALL  0xffu

/*
 * MeggyJr RGB screen
 */
#define S_COLS 8
#define S_ROWS 8
#define S_FPS 124

#define S_C 2
#define S_V 1
#define S_PASSES ((1 << S_C) + (1 << S_V))
/* I must have been crazy when I thought this was a good idea! */
typedef union _pixel_t {
    struct {
        uint8_t r : S_C; /* red */
        uint8_t g : S_C; /* green */
        uint8_t b : S_C; /* blue */
        uint8_t v : S_V; /* value (luminosity) */
    } bits;
    uint8_t colour;
} pixel_t;

static pixel_t rgb_screen[S_COLS][S_ROWS];


#endif
