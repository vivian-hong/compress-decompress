/*
 *      compress40.c
 *      Authors: Vivian Hong and Erika Odmark
 *      Date: October 11, 2016
 *      HW4 Part A
 *
 *      Contains functions that compress a ppm image and print the compressed 
 *      image in the compressd format.
 */

#include <stdlib.h>
#include <pnm.h>
#include <stdint.h>
#include <seq.h>
#include <a2plain.h>
#include <bitpack.h>
#include <compress40.h>
#include "brightness.h"
#include "chromacity.h"

#define BYTE 8

#define A_WIDTH 6
#define B_WIDTH 6
#define C_WIDTH 6
#define D_WIDTH 6
#define PB_WIDTH 4
#define PR_WIDTH 4
#define A_LSB 26
#define B_LSB 20
#define C_LSB 14
#define D_LSB 8
#define PB_LSB 4
#define PR_LSB 0

Pnm_ppm read_decompressed_image(FILE *image, A2Methods_T methods);
void make_even(Pnm_ppm image);
void compress_2x2(int col, int row, Pnm_ppm image, 
                      A2Methods_T methods);
uint64_t pack_bits(Compress_info compress_info);
void write_codeword(uint64_t codeword);


/*
 * Entry point to this module. Compresses the image read in from the given
 *         file pointer and prints the final compressed image format to stdout.
 */
void compress40(FILE *fp)
{
        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm image = Pnm_ppmread(fp, methods);
        make_even(image);

        /* header of compressed image */
        fprintf(stdout, "COMP40 Compressed image format 2\n%u %u\n", 
                image -> width, image -> height);

        for (int row = 0; row < (int)(image -> height); row += 2) {
                for (int col = 0; col < (int)(image -> width); col += 2) {
                        compress_2x2(col, row, image, methods);
                }
        }
        Pnm_ppmfree(&image);
}


/*
 * If width or height is odd, remove the last col or row respectively.
 * If changed, width and height in the Pnm_ppm struct will no longer be the
 *      same as width and height stored in the A2Methods UArray2. So, only
 *      access width and height from struct after this function has been
 *      called.
 */
void make_even(Pnm_ppm image)
{
        if (image -> width %2 != 0) {
                (image -> width)--;
        }
        if (image -> height %2 != 0) {
                (image -> height)--;
        }
}

/*
 * Reads a 2 by 2 block of pixels from image and compresses pixel information
 *         into a uint64_t word.
 */
void compress_2x2(int col, int row, Pnm_ppm image,
                     A2Methods_T methods)
{
        uint64_t codeword;
        Compress_info compress_info = new_compress_info();
        compress_brightness(image, col, row, methods, compress_info);
        compress_chromacity(image, col, row, methods, compress_info);
        codeword = pack_bits(compress_info);
        write_codeword(codeword);

        free(compress_info);
}

/*
 * Packs 2 by 2 pixel information into a uint64_t codeword and returns it.
 */
uint64_t pack_bits(Compress_info compress_info)
{
        uint64_t codeword = 0;
        codeword = Bitpack_newu(codeword, A_WIDTH, A_LSB, 
                                 compress_info -> avg_brightness);
        codeword = Bitpack_news(codeword, B_WIDTH, B_LSB, 
                                 compress_info -> top_bottom_brightness);
        codeword = Bitpack_news(codeword, C_WIDTH, C_LSB, 
                                 compress_info -> left_right_brightness);
        codeword = Bitpack_news(codeword, D_WIDTH, D_LSB, 
                                 compress_info -> diagonal_brightness);
        codeword = Bitpack_newu(codeword, PB_WIDTH, PB_LSB, 
                                 compress_info -> Pb_avg);
        codeword = Bitpack_newu(codeword, PR_WIDTH, PR_LSB, 
                                 compress_info -> Pr_avg);
        return codeword;
}

/*
 * Writes the codeword to stdout in Big Endian order.
 */
void write_codeword(uint64_t codeword)
{
        putchar(Bitpack_getu(codeword, BYTE, BYTE * 3));
        putchar(Bitpack_getu(codeword, BYTE, BYTE * 2));
        putchar(Bitpack_getu(codeword, BYTE, BYTE));
        putchar(Bitpack_getu(codeword, BYTE, 0));
}

#undef BYTE 
#undef A_WIDTH 
#undef B_WIDTH 
#undef C_WIDTH 
#undef D_WIDTH 
#undef PB_WIDTH 
#undef PR_WIDTH 
#undef A_LSB 
#undef B_LSB 
#undef C_LSB 
#undef D_LSB 
#undef PB_LSB 
#undef PR_LSB
