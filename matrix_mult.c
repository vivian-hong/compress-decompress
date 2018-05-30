/*      
 *      matrix_mult.c
 *      Authors: Erika Odmark and Vivian Hong
 *      Date: October 12, 2016
 *      HW4
 *
 *      Functions that perform the matrix multiplications needed in compression
 *      and decompression of an image
 */

#include <stdlib.h>
#include "matrix_mult.h"

/*
 * Returns the result of a dot product between a RGB pixel and the specific 
 *      conversion array provided
 */
float multiply_compress(Pnm_rgb pixel, float* conversion, int denom)
{
        return conversion[0] * ((float)pixel -> red / denom) 
                + conversion[1] * ((float)pixel -> green / denom) 
                + conversion[2] * ((float)pixel -> blue / denom);
}

/*
 * Returns the result of a dot product between the component video information
 *      and the specific conversion array provided
 */
unsigned multiply_decompress(float brightness, float Pb, float Pr, 
                          float* conversion, int denom)
{
        /* Does not do a full matrix muliplication because the first scalar 
         * constant will always be one 
         */
        float RGB_val = (brightness + conversion[0] * Pb + conversion[1] * Pr) 
                * denom;
        if (RGB_val < 0) {
                return 0;
        }
        return RGB_val; 
}

