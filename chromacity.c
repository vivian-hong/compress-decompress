/*      
 *      chromacity.c
 *      Authors: Erika Odmark and Vivian Hong
 *      Date: October 12, 2016
 *      HW 4
 *
 *      Contains functions to compress and decompress the chromacity of a 2x2 
 *      block of pixels from and to a ppm image
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chromacity.h"
#include "convert_signed_unsigned.h"
#include "matrix_mult.h"

float convert_Pb[3] = {-0.168736, -0.331264, 0.5};
#define CONVERT_PB convert_Pb

float convert_Pr[3] = {0.5, -0.418688, -0.081312};
#define CONVERT_PR convert_Pr

#define BLOCK_NUM 4

unsigned compress_avg_Pb(Pnm_ppm image, int col, int row, A2Methods_T methods);
float compress_Pb(Pnm_ppm image, int col, int row, A2Methods_T methods);
unsigned compress_avg_Pr(Pnm_ppm image, int col, int row, A2Methods_T methods);
float compress_Pr(Pnm_ppm image, int col, int row, A2Methods_T methods);
float decomp_Pb(Compress_info compress_info);
float decomp_Pr(Compress_info compress_info);


/*
 * Extracts chromacity information from the original image, calculates the
 *         info needed to pack, and stores it in the given compress_info
 *         struct.
 */
void compress_chromacity(Pnm_ppm original_image, int col, int row, 
                             A2Methods_T methods, Compress_info compress_info)
{
        compress_info -> Pb_avg = 
                compress_avg_Pb(original_image, col, row, methods);
        compress_info -> Pr_avg = 
                compress_avg_Pr(original_image, col, row, methods);
}


/*
 * Calculates the average Pb value of a 2 by 2 block of pixels.
 * Returns the unsigned index of the average Pb value for the block. 
 */
unsigned compress_avg_Pb(Pnm_ppm image, int col, int row,
                            A2Methods_T methods)
{
        return float_to_unsigned_index((compress_Pb(image, col, row, methods) 
                                        + compress_Pb(image, col + 1, row, 
                                                      methods) 
                                        + compress_Pb(image, col, row + 1, 
                                                      methods) 
                                        + compress_Pb(image, col + 1, row + 1, 
                                                      methods)) / BLOCK_NUM);
}

/*
 * Calculates and returns a Pb value for a single pixel.
 */
float compress_Pb(Pnm_ppm image, int col, int row, A2Methods_T methods)
{
        return multiply_compress(methods -> at(image -> pixels, col, row), 
                                 CONVERT_PB, image -> denominator);
}

/*
 * Calculates the average Pr value of a 2 by 2 block of pixels.
 * Returns the unsigned index of the average Pr value for the block. 
 */
unsigned compress_avg_Pr(Pnm_ppm image, int col, int row, A2Methods_T methods)
{
        return float_to_unsigned_index((compress_Pr(image, col, row, methods) 
                                        + compress_Pr(image, col + 1, row, 
                                                      methods) 
                                        + compress_Pr(image, col, row + 1, 
                                                      methods) 
                                        + compress_Pr(image, col + 1, row + 1, 
                                                      methods)) / BLOCK_NUM);
}

/*
 * Calculates and returns a Pr value for a single pixel.
 */
float compress_Pr(Pnm_ppm image, int col, int row,  A2Methods_T methods)
{
        return multiply_compress(methods -> at(image -> pixels, col, row), 
                                 CONVERT_PR, image -> denominator);
}

/*
 * Extracts chromacity information from the compressed image, calculates the
 *         info needed to create decompressed image, and stores it in the given
 *         decompress_info struct.
 */
void decompress_chromacity(Compress_info compress_info, 
                Decompress_info decomp_info)
{
        decomp_info -> Pb_avg = decomp_Pb(compress_info);
        decomp_info -> Pr_avg = decomp_Pr(compress_info);
}

/* 
 * Translates the average Pb from the compressed form to the form needed to 
 *         calculate the RGB pixels
 */
float decomp_Pb(Compress_info compress_info)
{
        return unsigned_index_to_float(compress_info -> Pb_avg);
}

/* 
 * Translates the Pr from the compressed form to the form needed to 
 *         calculate the RGB pixels
 */
float decomp_Pr(Compress_info compress_info)
{
        return unsigned_index_to_float(compress_info -> Pr_avg);
}


#undef CONVERT_PR
#undef CONVERT_PB
#undef BLOCK_NUM
