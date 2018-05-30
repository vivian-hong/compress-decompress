/*         compress_struct.c
 *         Authors: Erika Odmark and Vivian Hong
 *         Date: October 12, 2016
 *         HW4
 *
 *         Initializes the Compress_info struct
 */

#include <stdlib.h>
#include "compress_struct.h"

/*
 * Creates and initializes Compress_info struct.
 */
Compress_info new_compress_info()
{
        Compress_info compress_info = malloc(sizeof(*compress_info));
        compress_info -> Pb_avg = 0;
        compress_info -> Pr_avg = 0;
        compress_info -> avg_brightness = 0;
        compress_info -> top_bottom_brightness = 0;
        compress_info -> left_right_brightness = 0;
        compress_info -> diagonal_brightness = 0;

        return compress_info;
}
