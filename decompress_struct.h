/* 
 *         decompress_struct.h
 *         Authors: Erika Odmark and Vivian Hong
 *         Date: October 12, 2016
 *         HW4
 *
 *         Header file for Decompress_info struct
 */

#ifndef DECOMPRESS_STRUCT_H
#define DECOMPRESS_STRUCT_H

typedef struct Decompress_info {
        float Pb_avg;
        float Pr_avg;
        float brightness1;
        float brightness2;
        float brightness3;
        float brightness4;
} *Decompress_info;

Decompress_info new_decompress_info();

#endif /* DECOMPRESS_STRUCT_H */
