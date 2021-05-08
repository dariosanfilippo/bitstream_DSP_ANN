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

void sine(int sr, double amp, double freq, double* out, size_t samples);

#endif
