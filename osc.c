#include "osc.h"

void sine(size_t sr, double amp, double freq, double phase, Sig* out, 
    size_t out_vec_id) {
    /* calculate incremental value */
    double incr = (TWOPI * freq) / (double) sr;
    double ph = phase;
    for (size_t i = 0; i < out->vec_len; i++) {
        out->vec_space[out_vec_id][i] = amp * sin(ph);
        /* increment phase and wrap around */
        ph += incr;
        if (ph >= TWOPI)
            ph -= TWOPI;
        if (ph < 0.0)
            ph += TWOPI;
    }
}
