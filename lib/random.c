/*
 * Note: random and srandom are not thread safe!
 */

/*
 * 2^31 - 1
 */
#define RANDOM_MAX 0x7fffffffUL
/*
 * Internal state of the random number generator. Choice of initial seed
 * promoted only by the Ultimate Answer to the Ultimate Question of Life, The
 * Universe, and Everything.
 */
static uint32_t next = 42;

/*
 * A simple Lehmer RNG
 *
 * Operates on the multiplicative group of integers modulo RANDOM_MAX.
 * The following code calculates
 *     x = (7^5 * x) % RANDOM_MAX
 * in 32-bit arithmetic without overflowing, and is taken from:
 *
 * Park, S. K., and K. W. Miller. 1988. "Random Number Generators: Good Ones
 * Are Hard to Find." Communications of the ACM 31 (10): 1192–1201.
 * doi:10.1145/63039.63042.
 *
 * Declared static to prevent exporting this symbol.
 */
static uint32_t _random(uint32_t *context) {
    int32_t high, low, x;

    high = *context / 127773L;
    low  = *context % 127773L;
    x = 16807L * low - 2836L * high;
    if (x <= 0) {
        x += RANDOM_MAX
    *context = x;
    return (x % (RANDOM_MAX + 1));
}

/*
 * Generate a random 32bit number in the range [0..RAND_MAX)
 */
uint32_t random(void) {
    return _random(&next);
}

/*
 * Update the internal state of the random number generator to a
 * pre-determined value
 */
void srandom(uint32_t seed) {
    next = seed;
}

/*
 * Re-entrant (thread safe) version of random()
 */
uint32_t random_r(uint32_t *context) {
    return _random(context);
}
