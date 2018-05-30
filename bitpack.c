/*
 *        bitpack.c
 *        Authors: Erika Odmark and Vivian Hong
 *        Date: October 12, 2016
 *        HW4
 *
 *       Contains functions that get and place a given number of bits into a 
 *       uint64_t. Supports signed and unsigned ints.
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <bitpack.h>
#include "except.h"

#define MAX_WIDTH 64

Except_T Bitpack_Overflow = { "Overflow packing bits"};

/*
 * Shifts an unsigned int, "value", left by number of bits, "shift".
 * Allows for a 64 bit shift. 
 */
static inline uint64_t shift_u_left(uint64_t value, uint64_t shift)
{
        if (shift == MAX_WIDTH) {
                return 0;
        }
        return value << shift;
}
/*
 * Shifts an unsigned int, "value", right by number of bits, "shift".
 * Allows for a 64 bit shift. 
 */
static inline uint64_t shift_u_right(uint64_t value, uint64_t shift)
{
        if (shift == MAX_WIDTH) {
               return 0;
        }
        return value >> shift;
}

/*
 * Shifts a signed int, "value", left by number of bits, "shift".
 * Allows for a 64 bit shift. 
 */
static inline int64_t shift_s_left(int64_t value, int64_t shift)
{
        if (shift == MAX_WIDTH) {
                return 0;
        }
        return value << shift;
}

/*
 * Shifts a signed int, "value", right by number of bits, "shift".
 * Allows for a 64 bit shift. 
 */
static inline int64_t shift_s_right(int64_t value, int64_t shift)
{
        if (shift == MAX_WIDTH) {
                if (value < 0) {
                        return -1;
                } else {
                        return 0;
                }
        }
        return value >> shift;
}

uint64_t pow2(unsigned width);


/*
 * Returns true if the unsigned value given fits in the provided number of bits.
 * Takes the unsigned value, n, to fit, and the number of bits, width.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        if (n == 0 && width == 0) {
                return true;
        }
        if (pow2(width) > n) {
                return true;
        }
        return false;
}

/*
 * Returns true if the signed value given fits in the provided number of bits.
 * Takes the signed value, "n", to fit, and the number of bits, "width".
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        if (n == 0 && width == 0) {
                return true;
        }
        if (n <= 0 && (shift_s_right(pow2(width) + 2, 1)) > abs(n)) {
                        return true;
        } else if (n > 0 && (shift_s_right(pow2(width), 1)) > n) {
                        return true;
        }
        return false;
}

/*
 * Returns the unsigned value of length width, at place lsb in the given word.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width + lsb <= MAX_WIDTH);
        if (width == 0) {
                return 0;
        }
        uint64_t mask = shift_u_left((pow2(width) - 1), lsb);
        return shift_u_right((word & mask), lsb);
}

/*
 * Returns the signed value of length width, at place lsb in the given word.
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width + lsb <= MAX_WIDTH);
        if (width == 0) {
                return 0;
        }
        uint64_t mask = shift_u_left((pow2(width) - 1), lsb);
        int64_t value = (word & mask);
        value = shift_s_left(value, MAX_WIDTH - width - lsb);
        return shift_s_right(value, MAX_WIDTH - width);
}

/*
 * Returns a copy of word with the given unsigned value stored at place lsb.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
        assert(width + lsb <= MAX_WIDTH);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        } else if (width == 0) {
                return word;
        }
        uint64_t mask = ~0 ^ shift_u_left(pow2(width) - 1, lsb);
        return (word & mask) | shift_u_left(value, lsb);
}

/*
 * Returns a copy of word with the given signed value stored at place lsb.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{
        assert(width + lsb <= MAX_WIDTH);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        if (value > 0) {
                return Bitpack_newu(word, width, lsb, value);
        } else if (width == 0) {
                return word;
        }

        uint64_t mask = ~0 ^ shift_u_left(pow2(width) - 1, lsb);
        uint64_t mask_value = (pow2(width) - 1);
        return (word & mask) | shift_s_left(value & mask_value, lsb);
}

/*
 * Helper function that returns 2 to the power of width.
 */
uint64_t pow2(unsigned width)
{
        return shift_u_left(2, width - 1);
}

#undef MAX_WIDTH
