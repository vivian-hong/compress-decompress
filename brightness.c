/*
 *      brightness.c
 *      Authors: Erika Odmark and Vivian Hong
 *      Date: October 12, 2016
 *      HW 4
 *
 *      Contains functions that compress and decompress the brightness of a 2x2
 *      block of pixels from and to a ppm image, respectively.
 */

#include <stdlib.h>
#include <stdint.h>
#include "brightness.h"
#include "convert_signed_unsigned.h"
#include "matrix_mult.h"

float convert[3] = {0.299, 0.587, 0.114};
#define CONVERT convert

#define BLOCK_NUM 4

Decompress_info populate_struct(Pnm_ppm image, int col, int row,
                                        A2Methods_T methods);
unsigned calc_avg(Decompress_info decomp_info);
signed calc_top_bottom(Decompress_info decomp_info);
signed calc_left_right(Decompress_info decomp_info);
signed calc_diagonal(Decompress_info decomp_info);
float calc_brightness1(Compress_info compress_info);
float calc_brightness2(Compress_info compress_info);
float calc_brightness3(Compress_info compress_info);
float calc_brightness4(Compress_info compress_info);


/*
 * Extracts the brightness information from a 2 by 2 block of pixels, and
 *         stores it in the given compress_info struct.
 */
void compress_brightness(Pnm_ppm image, int col, int row, 
                             A2Methods_T methods, Compress_info compress_info)
{
        Decompress_info decomp_info = populate_struct(image, col, row, methods);
        compress_info -> avg_brightness = calc_avg(decomp_info);
        compress_info -> top_bottom_brightness = calc_top_bottom(decomp_info);
        compress_info -> left_right_brightness = calc_left_right(decomp_info);
        compress_info -> diagonal_brightness = calc_diagonal(decomp_info);
        
        free(decomp_info);
}

/*
 * Populates the decompress_info stuct with brightness values from the input
 *         image for each pixel in the 2x2 block and returns the filled 
 *         decompress_info struct.
 */
Decompress_info populate_struct(Pnm_ppm image, int col, int row, 
                A2Methods_T methods)
{
        Decompress_info decomp_info = malloc(sizeof(*decomp_info));
        decomp_info -> brightness1 = 
                multiply_compress(methods -> at(image -> pixels, col, row), 
                                  CONVERT, image -> denominator);
        decomp_info -> brightness2 = 
                multiply_compress(methods -> at(image -> pixels, col + 1, row),
                                  CONVERT, image -> denominator);
        decomp_info -> brightness3 = 
                multiply_compress(methods -> at(image -> pixels, col, row + 1),
                                  CONVERT, image -> denominator);
        decomp_info -> brightness4 = 
                multiply_compress(methods -> 
                                  at(image -> pixels, col + 1, row + 1), CONVERT,
                                  image -> denominator);

        return decomp_info;
}

/*
 * Calculates the average brightness for the pixels in the 2x2 block, and
 *         returns this average in a float.
 */
unsigned calc_avg(Decompress_info decomp_info)
{
        return float_to_unsigned((decomp_info -> brightness4 
                                  + decomp_info -> brightness3 
                                  + decomp_info -> brightness2 
                                  + decomp_info -> brightness1) 
                                 / BLOCK_NUM);
}


/*
 * Calculates the gradient of brightness from top to bottom of the 2x2 pixel
 *         block, and returns it as a float.
 */
int calc_top_bottom(Decompress_info decomp_info)
{
        return float_to_signed((decomp_info -> brightness4 
                                + decomp_info -> brightness3 
                                - decomp_info -> brightness2 
                                - decomp_info -> brightness1) 
                               / BLOCK_NUM);
}

/*
 * Calculates the gradient of brightness from left to right of the 2x2 pixel
 *         block, and returns it as a float.
 */
int calc_left_right(Decompress_info decomp_info)
{
        return float_to_signed((decomp_info -> brightness4 
                                - decomp_info -> brightness3 
                                + decomp_info -> brightness2 
                                - decomp_info -> brightness1) 
                               / BLOCK_NUM);
}

/*
 * Calculates the gradient of brightness from the bottom right to the top left
 *          of the 2x2 pixel block, and returns it as a float.
 */
int calc_diagonal(Decompress_info decomp_info)
{
        return  float_to_signed((decomp_info -> brightness4 
                                 - decomp_info -> brightness3 
                                 - decomp_info -> brightness2 
                                 + decomp_info -> brightness1) 
                                / BLOCK_NUM);
}

/*
 * Takes brightness information from a 2 by 2 block of pixels, calculates the 
 *         decompressed brightness information, and stores it in the given 
 *         decomp_info struct.
 * Takes compress_info, which holds information about pixels in its compressed
 *         form, and decomp_info, to store the decompressed pixel info.
 */
void decompress_brightness(Compress_info compress_info,
                           Decompress_info decomp_info)
{
        decomp_info -> brightness1 = calc_brightness1(compress_info);
        decomp_info -> brightness2 = calc_brightness2(compress_info);
        decomp_info -> brightness3 = calc_brightness3(compress_info);
        decomp_info -> brightness4 = calc_brightness4(compress_info);
}

/*
 * Returns the brightness value of the upper left pixel in the 2 by 2 block.
 * Takes the compress_info that holds compressed information for the
 *         2 by 2 block of pixels.
 */
float calc_brightness1(Compress_info compress_info)
{

        return unsigned_to_float(compress_info -> avg_brightness) 
               - signed_to_float(compress_info -> top_bottom_brightness) 
               - signed_to_float(compress_info -> left_right_brightness) 
               + signed_to_float(compress_info -> diagonal_brightness);
}

/*
 * Returns the brightness value of the upper right pixel in the 2 by 2 block.
 * Takes the compress_info that holds compressed information for the
 *         2 by 2 block of pixels.
 */
float calc_brightness2(Compress_info compress_info)
{
        return unsigned_to_float(compress_info -> avg_brightness) 
               - signed_to_float(compress_info -> top_bottom_brightness) 
               + signed_to_float(compress_info -> left_right_brightness) 
               - signed_to_float(compress_info -> diagonal_brightness);
}

/*
 * Returns the brightness value of the lower left pixel in the 2 by 2 block.
 * Takes the compress_info that holds compressed information for the
 *         2 by 2 block of pixels.
 */
float calc_brightness3(Compress_info compress_info)
{
        return unsigned_to_float(compress_info -> avg_brightness) 
               + signed_to_float(compress_info -> top_bottom_brightness) 
               - signed_to_float(compress_info -> left_right_brightness) 
               - signed_to_float(compress_info -> diagonal_brightness);
}

/*
 * Returns the brightness value of the lower right pixel in the 2 by 2 block.
 * Takes the compress_info that holds compressed information for the
 *         2 by 2 block of pixels.
 */
float calc_brightness4(Compress_info compress_info)
{
        return unsigned_to_float(compress_info -> avg_brightness) 
               + signed_to_float(compress_info -> top_bottom_brightness) 
               + signed_to_float(compress_info -> left_right_brightness) 
               + signed_to_float(compress_info -> diagonal_brightness);
}

#undef CONVERT
#undef BLOCK_NUM
