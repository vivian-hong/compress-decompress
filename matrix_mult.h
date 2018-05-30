/*      matrix_mult.h
 *      Authors: Erika Odmark and Vivian Hong
 *      Date: October 12, 2016
 *      HW4
 *
 *      Header file for matrix multiplication functions
 */

#ifndef MATRIX_MULT_H
#define MATRIX_MULT_H

#include <pnm.h>

float multiply_compress(Pnm_rgb pixel, float* conversion_array, int denom);
unsigned multiply_decompress(float brightness, float Pb, float Pr, 
                          float* conversion, int denom);

#endif /* MATRIX_MULT_H */

