/*
 *         convert_signed_unsigned.c
 *         Authors: Vivian Hong and Erika Odmark
 *         Date: October 16, 2016
 *         HW4
 *
 *         Contains functions that convert floats to and from thier signed and 
 *         unsigned representations (including the index of certain floats)
 *         within specified bounds.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "convert_signed_unsigned.h"

#define MAX_UNSIGNED 63
#define MIN_CHROMA -0.3
#define MAX_CHROMA 0.3
#define CHROMA_SCALE 102

#define PBPR_ARRAY_LEN 16
float pbpr_array[PBPR_ARRAY_LEN] = {0.35, -0.35, 0.20, -0.20, 0.15, -0.15, 0.10,
                        -0.10, 0.077, -0.077, 0.055, -0.055, 0.033,
                        -0.033, 0.011, -0.011};
#define PBPR_ARRAY pbpr_array



/*
 * Converts a float in [0, 1] to a 9 bit unsigned integer.
 */
unsigned int float_to_unsigned(float value)
{
        return (unsigned)round(value * MAX_UNSIGNED);
}

/*
 * Converts a float in [-0.3, 0.3] to a 5 bit signed integer. If the input
 *         is greater than 0.3 or less than -0.3, the float is converted to
 *         0.3 and -0.3 respectively.
 */
int float_to_signed(float value)
{
        if (value < MIN_CHROMA) {
                return round(MIN_CHROMA * CHROMA_SCALE);
        } else if (value > MAX_CHROMA) {
                return round(MAX_CHROMA * CHROMA_SCALE);
        } else {
                return round(value * CHROMA_SCALE);
        }
}

/*
 * Takes in a float and returns the index of the value in the pbpr_array that
 *         best approximates the input.
 */
unsigned int float_to_unsigned_index(float value)
{
        float diff = fabsf(value - PBPR_ARRAY[0]);
        int index = 0;
        for (int i = 1; i < PBPR_ARRAY_LEN; i++) {
                if (fabsf(value - PBPR_ARRAY[i]) < diff) {
                        diff = fabsf(value - PBPR_ARRAY[i]);
                        index = i;
                }
        }
        return (unsigned)index;

}

/*
 * Returns the float stored in the PBPR_ARRAY at index given.
 */
float unsigned_index_to_float(unsigned int index) 
{
        return PBPR_ARRAY[index];
}


/*
 * Converts a 9 bit unsigned integer to a float in [0, 1].
 */
float unsigned_to_float(unsigned value)
{
        return (float)(value) / MAX_UNSIGNED;
}

/*
 * Converts a 5 bit signed integer in [-15, 15] to a float in [-0.3, 0.3].
 */
float signed_to_float(int value)
{
        return (float)value / CHROMA_SCALE;
}




#undef MAX_UNSIGNED
#undef MIN_CHROMA
#undef MAX_CHROMA
#undef CHROMA_SCALE
#undef PBPR_ARRAY_LEN
#undef PBPR_ARRAY


