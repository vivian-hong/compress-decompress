/* 
 *         compress_struct.h
 *         Authors: Erika Odmark and Vivian Hong
 *         Date: October 12, 2016
 *         HW4
 *
 *         Header file for Compress_info struct
 */

#ifndef COMPRESS_STRUCT_H
#define COMPRESS_STRUCT_H

typedef struct Compress_info {

        unsigned Pb_avg;              /* 4 bits */
        unsigned Pr_avg;              /* 4 bits */
        unsigned avg_brightness;      /* 9 bits */
        signed top_bottom_brightness; /* 5 bits */
        signed left_right_brightness; /* 5 bits */
        signed diagonal_brightness;   /* 5 bits */

} *Compress_info;

Compress_info new_compress_info();

#endif /* COMPRESS_STRUCT_H */
