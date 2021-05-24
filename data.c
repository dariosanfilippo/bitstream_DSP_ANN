#include "data.h"

void freq_est_data(Sig* in, Sig* out) {
    assert(in->space_dim == out->space_dim);

    audio freq;
    audio amp;
    audio phase;

    for (size_t i = 0; i < in->space_dim; i++) {
        freq = (random() / RAND_MAX) * 5000.0;
        amp = (random() / RAND_MAX) * .5;
        phase = (random() / RAND_MAX) * TWOPI;
        out->vec_space[i][0] = freq;
        sine(in->sr, amp, freq, phase, in, i);
        dsm3(in, in, i, i);
    }
}




