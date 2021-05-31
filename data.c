#include "data.h"

void freq_est_data(Sig* in, Sig* out) {
    assert(in->space_dim == out->space_dim);

    audio freq;
    audio amp;
    audio phase;

    for (size_t i = 0; i < in->space_dim; i++) {
        freq = frand() * 20000.0;
        amp = frand() * .5 + .25;
        phase = frand() * TWOPI;
        out->vec_space[i][0] = freq;
        sine(in->sr, amp, freq, phase, in, i);
        dsm3(in, in, i, i);
    }
}

void amp_est_data(Sig* in, Sig* out) {
    assert(in->space_dim == out->space_dim);

    audio freq;
    audio amp;
    audio phase;

    for (size_t i = 0; i < in->space_dim; i++) {
        freq = frand() * 20000.0;
        amp = frand() * .5 + .25;
        phase = frand() * TWOPI;
        out->vec_space[i][0] = amp;
        sine(in->sr, amp, freq, phase, in, i);
        dsm3(in, in, i, i);
    }
}

void adder_data(Sig* in, Sig* out) {
    assert(in->space_dim == out->space_dim);

    audio freq0;
    audio freq1;
    audio amp0;
    audio amp1;
    audio phase0;
    audio phase1;
    Sig* in0 = malloc(sizeof(Sig));
    Sig* in1 = malloc(sizeof(Sig));
    sig_alloc(in0, in->space_dim, in->vec_len / 2, in->sr);
    sig_alloc(in1, in->space_dim, in->vec_len / 2, in->sr);

    for (size_t i = 0; i < in->space_dim; i++) {
        freq0 = frand() * 5000.0;
        freq1 = frand() * 5000.0;
        amp0 = frand() * .25;
        amp1 = frand() * .25;
        phase0 = frand() * TWOPI;
        phase1 = frand() * TWOPI;
        sine(in0->sr, amp0, freq0, phase0, in0, i);
        sine(in1->sr, amp1, freq1, phase1, in1, i);
        for (size_t j = 0; j < out->vec_len; j++) {
            out->vec_space[i][j] = in0->vec_space[i][j] + in1->vec_space[i][j];
        }
        dsm3(in0, in0, i, i);
        dsm3(in1, in1, i, i);
        dsm3(out, out, i, i);
        for (size_t j = 0; j < in0->vec_len; j++) {
            in->vec_space[i][j] = in0->vec_space[i][j];
            in->vec_space[i][j + in1->vec_len] = in1->vec_space[i][j];
        }
    }

    sig_free(in0);
    sig_free(in1);
}

void mul_data(Sig* in, Sig* out) {
    assert(in->space_dim == out->space_dim);

    audio freq0;
    audio freq1;
    audio amp0;
    audio amp1;
    audio phase0;
    audio phase1;
    Sig* in0 = malloc(sizeof(Sig));
    Sig* in1 = malloc(sizeof(Sig));
    sig_alloc(in0, in->space_dim, in->vec_len / 2, in->sr);
    sig_alloc(in1, in->space_dim, in->vec_len / 2, in->sr);

    for (size_t i = 0; i < in->space_dim; i++) {
        freq0 = frand() * 5000.0;
        freq1 = frand() * 5000.0;
        amp0 = frand() * .5;
        amp1 = frand() * .5;
        phase0 = frand() * TWOPI;
        phase1 = frand() * TWOPI;
        sine(in0->sr, amp0, freq0, phase0, in0, i);
        sine(in1->sr, amp1, freq1, phase1, in1, i);
        for (size_t j = 0; j < out->vec_len; j++) {
            out->vec_space[i][j] = in0->vec_space[i][j] * in1->vec_space[i][j];
        }
        dsm3(in0, in0, i, i);
        dsm3(in1, in1, i, i);
        dsm3(out, out, i, i);
        for (size_t j = 0; j < in0->vec_len; j++) {
            in->vec_space[i][j] = in0->vec_space[i][j];
            in->vec_space[i][j + in1->vec_len] = in1->vec_space[i][j];
        }
    }

    sig_free(in0);
    sig_free(in1);
}

