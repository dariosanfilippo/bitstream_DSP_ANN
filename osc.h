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

void sine(size_t sr, double amp, double freq, double* out, size_t samples);

#endif
