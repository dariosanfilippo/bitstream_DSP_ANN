#include "osc.h"

void sine(size_t sr, double amp, double freq, double* out, size_t samples) {
    /* calculate incremental value */
    double incr = (TWOPI * freq) / (double)sr;
    double ph = 0.0;
    for (int i = 0; i < samples; i++) {
        out[i] = amp * sin(ph);
        /* increment phase and wrap around */
        ph += incr;
        if (ph >= TWOPI)
            ph -= TWOPI;
        if (ph < 0.0)
            ph += TWOPI;
    }
}
