/* 
 *      decompress40.c
 *      Authors: Vivian Hong and Erika Odmark
 *      Date: October 11, 2016
 *      HW4 Part A
 *
 *      Contains functions that decompress an image from the compressed format to
 *      a ppm image and print the resulting image.
 */

#include <stdlib.h>
#include <a2plain.h>
#include <stdint.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>
#include <compress40.h>
#include "brightness.h"
#include "chromacity.h"
#include "matrix_mult.h"

#define DENOMINATOR 255

float convert_red[2] = {0, 1.402};
float convert_green[2] = {-0.344136, -0.714136};
float convert_blue[2] = {1.772, 0};
#define CONVERT_RED convert_red
#define CONVERT_GREEN convert_green
#define CONVERT_BLUE convert_blue

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

typedef struct Dimensions {
        unsigned height;
        unsigned width;
} Dimensions;

Dimensions read_header(FILE *image);
Pnm_ppm initialize_compressed_image(Dimensions dimensions, A2Methods_T methods);
uint64_t read_codeword(FILE *image);
void decompress_2x2(Pnm_ppm decomp_image, uint64_t codeword, int col, 
                    int row, A2Methods_T methods);
void unpack_bits(Compress_info compress_info, uint64_t codeword);
void insert_2x2(Pnm_ppm decomp_image, Decompress_info decomp_info, int col, 
                int row, A2Methods_T methods);
Pnm_rgb calc_pixel(float brightness, float Pb, float Pr);
void insert_pixel(Pnm_rgb to_add, Pnm_ppm decomp_image, int col, int row, 
                  A2Methods_T methods);

/*
 * Entry point to this module. Decompresses the image read in from the given
 *         file pointer and prints the final ppm image to stdout.
 */
void decompress40(FILE *image)
{
        A2Methods_T methods = uarray2_methods_plain;
        Dimensions dimensions = read_header(image);
        Pnm_ppm decomp_image = initialize_compressed_image(dimensions, methods);
        uint64_t codeword;

        /* checks for empty image */
        if (decomp_image -> width == 0 || decomp_image -> height == 0) {
                /* prints empty image */
                fprintf(stdout, "P6\n%i %i\n", 
                        decomp_image -> width, decomp_image ->height);
                Pnm_ppmfree(&decomp_image);
                exit(EXIT_SUCCESS);
        }

        /* decopresses each codeword to a 2x2 block of pixels */
        for (int row = 0; row < (int)dimensions.height; row += 2) {
                for (int col = 0; col < (int)dimensions.width; col += 2){
                        codeword = read_codeword(image);
                        decompress_2x2(decomp_image, codeword, 
                                       col, row, methods);
                }
        }

        /* prints decompressed image */
        Pnm_ppmwrite(stdout, decomp_image);

        Pnm_ppmfree(&decomp_image);
}

/*
 * Populates a the Dimesions struct with width and height from the header and 
 *         returns a copy of it.
 */
Dimensions read_header(FILE *image)
{
        Dimensions dimensions;
        int read = fscanf(image, "COMP40 Compressed image format 2\n%u %u", 
                           &dimensions.width, &dimensions.height);
        assert(read == 2); 
        int c = getc(image);
        assert(c == '\n');
        return dimensions;
}

/*
 * Returns a new Pnm_ppm initialized with the width and height stored in the
 *        given Dimenions struct.
 */
Pnm_ppm initialize_compressed_image(Dimensions dimensions, A2Methods_T methods)
{
        
        Pnm_ppm decomp_image = malloc(sizeof(*decomp_image));
        decomp_image -> width = dimensions.width;
        decomp_image -> height = dimensions.height;
        decomp_image -> denominator = DENOMINATOR;
        decomp_image -> pixels = methods -> new(decomp_image -> width, 
                                 decomp_image -> height, sizeof(struct Pnm_rgb));
        decomp_image -> methods = methods;
        return decomp_image;
}

/*
 * Reads in a codeword from the file pointer in Big Endian order and returns the
 *         full codeword.
 */
uint64_t read_codeword(FILE *image)
{
        uint64_t codeword = 0;
        codeword = Bitpack_newu(codeword, BYTE, BYTE * 3, getc(image));
        codeword = Bitpack_newu(codeword, BYTE, BYTE * 2, getc(image));
        codeword = Bitpack_newu(codeword, BYTE, BYTE, getc(image));
        codeword = Bitpack_newu(codeword, BYTE, 0, getc(image));
        return codeword;
}

/*
 * Decompresses the given codeword to a 2x2 block of pixels and places it in
 *        the final image at the correct indices.
 */
void decompress_2x2(Pnm_ppm decomp_image, uint64_t codeword, int col, 
                    int row, A2Methods_T methods)
{
        Compress_info compress_info = new_compress_info();
        Decompress_info decomp_info = new_decompress_info();
        unpack_bits(compress_info, codeword);
        decompress_brightness(compress_info, decomp_info);
        decompress_chromacity(compress_info, decomp_info);
        insert_2x2(decomp_image, decomp_info, col, row, methods);
        free(compress_info);
        free(decomp_info);
}

/*
 * Unpacks the information stored in codeword and populates the Compress_info 
 *         struct with information about the compressed image.
 */
void unpack_bits(Compress_info compress_info, uint64_t codeword)
{
        compress_info -> avg_brightness
                = Bitpack_getu(codeword, A_WIDTH, A_LSB);
        compress_info -> top_bottom_brightness  
                = Bitpack_gets(codeword, B_WIDTH, B_LSB);
        compress_info -> left_right_brightness
                = Bitpack_gets(codeword, C_WIDTH, C_LSB);
        compress_info -> diagonal_brightness 
                = Bitpack_gets(codeword, D_WIDTH, D_LSB);
        compress_info -> Pb_avg 
                = Bitpack_getu(codeword, PB_WIDTH, PB_LSB);
        compress_info -> Pr_avg 
                = Bitpack_getu(codeword, PR_WIDTH, PR_LSB);

}

/* 
 * Inserts four pixles into the ppm image based on the information given in the 
 *      decomp_info struct.
 */
void insert_2x2(Pnm_ppm decomp_image, Decompress_info decomp_info, int col, 
                int row, A2Methods_T methods)
{
        /* calculates each individual pixel in the 2x2 block */
        Pnm_rgb pixel1 = calc_pixel(decomp_info -> brightness1, 
                                    decomp_info -> Pb_avg, 
                                    decomp_info -> Pr_avg);
        Pnm_rgb pixel2 = calc_pixel(decomp_info -> brightness2, 
                                    decomp_info -> Pb_avg, 
                                    decomp_info -> Pr_avg);
        Pnm_rgb pixel3 = calc_pixel(decomp_info -> brightness3, 
                                    decomp_info -> Pb_avg, 
                                    decomp_info -> Pr_avg);
        Pnm_rgb pixel4 = calc_pixel(decomp_info -> brightness4, 
                                    decomp_info -> Pb_avg, 
                                    decomp_info -> Pr_avg);

        /* inserts each calculated pixel into the image */
        insert_pixel(pixel1, decomp_image, col, row, methods);
        insert_pixel(pixel2, decomp_image, col + 1, row, methods);
        insert_pixel(pixel3, decomp_image, col, row + 1, methods);
        insert_pixel(pixel4, decomp_image, col + 1, row + 1, methods);
}

/*
 * Calculates red, green, and blue for a pixel, given the component video values
 * Returns a the Pnm_rgb.
 */
Pnm_rgb calc_pixel(float brightness, float Pb, float Pr)
{
        Pnm_rgb pixel = malloc(sizeof(*pixel));
        pixel -> red = multiply_decompress(brightness, Pb, Pr, CONVERT_RED, 
                                           DENOMINATOR);
        pixel -> green = multiply_decompress(brightness, Pb, Pr, CONVERT_GREEN,
                                             DENOMINATOR);
        pixel -> blue = multiply_decompress(brightness, Pb, Pr, CONVERT_BLUE, 
                                            DENOMINATOR);
        return pixel;
}

/* 
 * Inserts a given pixel at index (col, row) in the ppm image.
 */
void insert_pixel(Pnm_rgb to_add, Pnm_ppm decomp_image, int col, int row, 
                  A2Methods_T methods)
{
        Pnm_rgb curr_pixel = (Pnm_rgb)methods 
                -> at(decomp_image -> pixels, col, row);

        /* add red */
        if (to_add -> red > DENOMINATOR) { /* check for overflow */
                curr_pixel -> red = DENOMINATOR;
        }  else {
                curr_pixel -> red = to_add -> red;
        }
        /* add green */
        if (to_add -> green > DENOMINATOR) {
                curr_pixel -> green = DENOMINATOR;
        } else {
                curr_pixel -> green = to_add -> green;
        }
        /* add blue */
        if (to_add -> blue > DENOMINATOR) {
                curr_pixel -> blue = DENOMINATOR;
        } else {
                curr_pixel -> blue = to_add -> blue;
        }

        free(to_add);
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

#undef DENOMINATOR
#undef CONVERT_RED 
#undef CONVERT_GREEN 
#undef CONVERT_BLUE 
