#ifndef MUSIC_H
#define MUSIC_H

/*
 * Note divisors, only provided names for sharp accidentals. Calculated based
 * on well-tempered tuning of pow(2, (n - 49)/12) * 440Hz. Names are given in
 * scientific pitch notation.
 */
#define tC8     956 /* 4186.01 Hz */
#define tB7    1012 /* 3951.07 Hz */
#define tAs7   1073 /* 3729.31 Hz */
#define tA7    1136 /* 3520    Hz */
#define tGs7   1204 /* 3322.44 Hz */
#define tG7    1276 /* 3135.96 Hz */
#define tFs7   1351 /* 2959.96 Hz */
#define tF7    1432 /* 2793.83 Hz */
#define tE7    1517 /* 2637.02 Hz */
#define tDs7   1607 /* 2489.02 Hz */
#define tD7    1703 /* 2349.32 Hz */
#define tCs7   1804 /* 2217.46 Hz */
#define tC7    1911 /* 2093    Hz */
#define tB6    2025 /* 1975.53 Hz */
#define tAs6   2145 /* 1864.66 Hz */
#define tA6    2273 /* 1760    Hz */
#define tGs6   2408 /* 1661.22 Hz */
#define tG6    2551 /* 1567.98 Hz */
#define tFs6   2703 /* 1479.98 Hz */
#define tF6    2863 /* 1396.91 Hz */
#define tE6    3034 /* 1318.51 Hz */
#define tDs6   3214 /* 1244.51 Hz */
#define tD6    3405 /* 1174.66 Hz */
#define tCs6   3608 /* 1108.73 Hz */
#define tC6    3822 /* 1046.5  Hz */
#define tB5    4050 /* 987.767 Hz */
#define tAs5   4290 /* 932.328 Hz */
#define tA5    4545 /* 880     Hz */
#define tGs5   4816 /* 830.609 Hz */
#define tG5    5102 /* 783.991 Hz */
#define tFs5   5405 /* 739.989 Hz */
#define tF5    5727 /* 698.456 Hz */
#define tE5    6067 /* 659.255 Hz */
#define tDs5   6428 /* 622.254 Hz */
#define tD5    6810 /* 587.33  Hz */
#define tCs5   7215 /* 554.365 Hz */
#define tC5    7645 /* 523.251 Hz */
#define tB4    8099 /* 493.883 Hz */
#define tAs4   8581 /* 466.164 Hz */
#define tA4    9091 /* 440     Hz */
#define tGs4   9631 /* 415.305 Hz */
#define tG4   10204 /* 391.995 Hz */
#define tFs4  10811 /* 369.994 Hz */
#define tF4   11454 /* 349.228 Hz */
#define tE4   12135 /* 329.628 Hz */
#define tDs4  12856 /* 311.127 Hz */
#define tD4   13621 /* 293.665 Hz */
#define tCs4  14431 /* 277.183 Hz */
#define tC4   15289 /* 261.626 Hz */
#define tB3   16198 /* 246.942 Hz */
#define tAs3  17161 /* 233.082 Hz */
#define tA3   18182 /* 220     Hz */
#define tGs3  19263 /* 207.652 Hz */
#define tG3   20408 /* 195.998 Hz */
#define tFs3  21622 /* 184.997 Hz */
#define tF3   22908 /* 174.614 Hz */
#define tE3   24270 /* 164.814 Hz */
#define tDs3  25713 /* 155.563 Hz */
#define tD3   27242 /* 146.832 Hz */
#define tCs3  28862 /* 138.591 Hz */
#define tC3   30578 /* 130.813 Hz */
#define tB2   32396 /* 123.471 Hz */
#define tAs2  34323 /* 116.541 Hz */
#define tA2   36364 /* 110     Hz */
#define tGs2  38526 /* 103.826 Hz */
#define tG2   40817 /* 97.9989 Hz */
#define tFs2  43244 /* 92.4986 Hz */
#define tF2   45815 /* 87.3071 Hz */
#define tE2   48540 /* 82.4069 Hz */
#define tDs2  51426 /* 77.7817 Hz */
#define tD2   54484 /* 73.4162 Hz */
#define tCs2  57724 /* 69.2957 Hz */
#define tC2   61156 /* 65.4064 Hz */
#define tB1   64793 /* 61.7354 Hz */

/*
 * XXX: This should be decoupled from just being a magic number somehow. Right
 * now it is exactly 84/500000, the number of ms that pass between each
 * TIMER2_COMPA_vect interrupt.
 */
#define MS_PERIODS 0.168f

void play_tone(uint16_t tone, float ms);

#endif
