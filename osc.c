#include "osc.h"

void sine(size_t sr, double amp, double freq, Sig* out) {
    /* calculate incremental value */
    double incr = (TWOPI * freq) / (double) sr;
    double ph = 0.0;
    for (size_t i = 0; i < out->vec_len; i++) {
        out->vec_space[0][i] = amp * sin(ph);
        /* increment phase and wrap around */
        ph += incr;
        if (ph >= TWOPI)
            ph -= TWOPI;
        if (ph < 0.0)
            ph += TWOPI;
    }
}
