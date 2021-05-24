/* 
 * *****************************************************************************
 *
 *      Oscillators
 *
 * *****************************************************************************
 */

#ifndef OSC
#define OSC

#include <math.h>

#define TWOPI 2.0 * M_PI

void sine(size_t sr, double amp, double freq, double phase, Sig* out, 
    size_t out_vec_id);

#endif
