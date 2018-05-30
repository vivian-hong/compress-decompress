/* 
 *      brightness.h
 *      Authors: Erika Odmark and Vivian Hong
 *      Date: October 12, 2016
 *      HW 4
 *
 *      Header file for compress and decompress brightness functions
 */

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H


#include <pnm.h>
#include "compress_struct.h"
#include "decompress_struct.h"

void compress_brightness(Pnm_ppm image, int col, int row, 
                         A2Methods_T methods, Compress_info compress_info);

void decompress_brightness(Compress_info compress_info, 
                           Decompress_info decomp_info);

#endif /* BRIGHTNESS_H */
