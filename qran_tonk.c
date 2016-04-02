// from tonc_core.h/.c
// A Quick (and dirty) random number generator and its seeder

#include "qran_tonk.h"

int __qran_seed= 42;     // Seed / rnd holder

// Seed routine
int sqran(int seed) {	
    int old= __qran_seed;
    __qran_seed= seed; 
    return old;	
}

//! Quick (and very dirty) pseudo-random number generator 
/*! \return random in range [0,8000h>
*/
inline int qran() {	
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}
