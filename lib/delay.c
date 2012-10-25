#define D_CLOCK_CYCLES 3
#define D_MICROSECONDS (F_CPU / (D_CLOCK_CYCLES * 1e6))

static void _delay_inner(uint8_t cycles) {
    asm volatile (
        "1: dec %0\n\t"
        "brne 1b\n\t"
        : "=r" (cycles)
        : "0" (cycles)
    );
}

void delay_us(double us) {
    uint8_t ticks;
    double t = D_MICROSECONDS * us;
    if (t < 1.0) {
        ticks = 1;
    } else {
        ticks = (uint8_t) t;
    }
    _delay_inner(ticks);
}

