/* 
 * *****************************************************************************
 *
 *      Oscillators
 *
 * *****************************************************************************
 */

#ifndef OSC
#define OSC

#define TWOPI 2.0 * M_PI

void sine(size_t sr, audio amp, audio freq, audio phase, Sig* out, 
    size_t out_vec_id);

#endif
