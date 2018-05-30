/* 
 *         convert_signed_unsigned.h
 *         Authors: Erika Odmark and Vivian Hong
 *         Date: October 12, 2016
 *         HW4
 *
 *         Contains header functions for converting between signed and unsigned
 *         values
 */

#ifndef CONVERT_SIGNED_UNSIGNED_H
#define CONVERT_SIGNED_UNSIGNED_H

#include <stdint.h>


unsigned int float_to_unsigned(float value);
int float_to_signed(float value);
unsigned int float_to_unsigned_index(float value);
float unsigned_index_to_float(unsigned int index);
float unsigned_to_float(unsigned value);
float signed_to_float(int value);

#endif /* CONVERT_SIGNED_UNSIGNED_H */
