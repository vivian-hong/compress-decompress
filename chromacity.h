/*      
 *      chromacity.h
 *      Authors: Erika Odmark and Vivian Hong
 *      Date: October 12, 2016
 *      HW 4
 *
 *      Header file for compress and decompress chromacity functions
 */

#ifndef CHROMACITY_H
#define CHROMACITY_H

#include <pnm.h>
#include "compress_struct.h"
#include "decompress_struct.h"

void compress_chromacity(Pnm_ppm original_image, int col, int row, 
                         A2Methods_T methods, Compress_info compress_info);
void decompress_chromacity(Compress_info compress_info, 
                           Decompress_info decomp_info);

#endif /* CHROMACITY_H */
