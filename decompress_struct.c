/*
 *         decompress_struct.c
 *         Authors: Erika Odmark and Vivian Hong
 *         Date: October 12, 2016
 *         HW4
 *
 *         Initializes the Decompress_info struct
 */

#include <stdlib.h>
#include "decompress_struct.h"

/*
 * Creates and initializes Decompress_info struct.
 */
Decompress_info new_decompress_info()
{
        Decompress_info decomp_info = malloc(sizeof(*decomp_info));
        decomp_info -> Pb_avg = 0;
        decomp_info -> Pr_avg = 0;
        decomp_info -> brightness1 = 0;
        decomp_info -> brightness2 = 0;
        decomp_info -> brightness3 = 0;
        decomp_info -> brightness4 = 0;
        return decomp_info;
}
