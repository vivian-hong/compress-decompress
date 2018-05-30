/*
 *      a2plain.c
 *      Authors: Andrew Savage and Vivian Hong
 *      Date: 10/5/16
 *      HW3 Part B
 *
 *      Implementation of A2Methods using Uarray2
 */

#include <stdlib.h>
#include <a2plain.h>
#include "uarray2.h"

typedef A2Methods_UArray2 A2;

/* Purpose: Returns a UArray2 as an A2Methods_UArray
 * Arguments: width, height, size
 * Returns: A2Methods_UArray
 */
static A2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/* Purpose: Returns a UArray2 as an A2Methods_UArray
 * Arguments: Width, height, size, blocksize (used but taken to be compatible
 *            for either A2Methods_Uarray2 type)
 * Returns: A2Methods_Uarray
 */
static A2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void)blocksize;
        return UArray2_new(width, height, size);
}

/* Purpose: Frees an A2Methods_UArray2
 * Arguments: Pointer to the A2Methods_UArray2
 * Returns: Nothing
 */
static void a2free(A2 *array2p)
{
        UArray2_free((UArray2_T *) array2p);
}

/* Purpose: Return width of the A2Methods_UArray2
 * Arguments: A2Methods_UArray2
 * Returns: Width
 */
static int width(A2 array2)
{
        return UArray2_width(array2);
}

/* Purpose: Return width of the A2Methods_UArray2
 * Arguments: A2Methods_UArray2
 * Returns: Height
 */
static int height(A2 array2)
{
        return UArray2_height(array2);
}

/* Purpose: Return size of elements in the A2Methods_UArray2
 * Arguments: A2Methods_UArray2
 * Returns: Size of a single element stored in the A2Methods_UArray2
 */
static int size(A2 array2)
{
        return UArray2_size(array2);
}

/* Purpose: Return blocksize of the A2Methods_UArray2
 * Arguments: A2Methods_UArray2
 * Returns: Blocksize
 */
 static int blocksize(A2 array2)
{
        (void)array2;
        return 0; /* TODO */
}

/* Purpose: Returns a pointer to the element stored in the A2Methods_UArray2
 *          at index (i,j)
 * Arguments: A2Methods_UArray2, col, row
 * Returns: Pointer to element stored at index (i,j)
 */
static A2Methods_Object *at(A2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}

/* Apply function used in map_row_major and map_col_major */
typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

/* Purpose: Map through every element in the A2Methods_UArray2 row-major and 
 *          use the apply function
 * Arguments: A2Methods_UArray2, pointer to a function, closure
 * Returns: Nothing
 */
static void map_row_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2_map_row_major(array2, (applyfun *) apply, cl);
}

/* Purpose: Map through every element in the A2Methods_UArray2 col-major and 
 *          use the apply function
 * Arguments: A2Methods_UArray2, pointer to a function, closure
 * Returns: Nothing
 */
static void map_col_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2_map_col_major(array2, (applyfun *) apply, cl);
} 

/* struct used in apply_small and small map functions */
struct a2fun_closure {
        A2Methods_smallapplyfun *apply;
        void *cl;
};

/* Purpose: Call the small apply function stored in the small closure
 * Arguments: Col, row, array, pointer to element stored at (col, row), closure
 * Returns: Nothing
 */
static void apply_small(int i, int j, UArray2_T array2, void *elem, void *vcl)
{
        struct a2fun_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)array2;
        cl->apply(elem, cl->cl);
}

/* Purpose: Maps through the A2Methods_UArray2 row-major using apply small 
 *          (making the mapping function compatible for apply functions that 
 *          take different arguments than map_row_major takes)
 * Arguments: A2Methods_UArray2, pointer to a function, closure
 * Returns: Nothing
 */
static void small_map_row_major(A2 a2, A2Methods_smallapplyfun apply,
                                  void *cl)
{
        struct a2fun_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/* Purpose: Maps through the A2Methods_UArray2 col-major using apply small 
 *          (making the mapping function compatible for apply functions that 
 *          take different arguments than map_col_major takes)
 * Arguments: A2Methods_UArray2, pointer to a function, closure
 * Returns: Nothing
 */
static void small_map_col_major(A2 a2, A2Methods_smallapplyfun apply,
                                  void *cl)
{
        struct a2fun_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/* struct that contains pointers to a2plain functions */
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,          
        map_col_major,                  
        NULL,                   // map_block_major
        map_row_major,          // map_default
        small_map_row_major,    // small_map_default            
        small_map_col_major,                    
        NULL,                   // small_map_block_major
        small_map_row_major,    // small_map_default
};

/* the exported pointer to the struct */

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
