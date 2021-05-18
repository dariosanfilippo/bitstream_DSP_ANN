/*******************************************************************************
 *
 *      This file contains first, second, and third-order delta-sigma
 *      modulators for multi-bit to one-bit conversion.
 *
 *      While the first-order modulator is stable for any signal in the 
 *      full-scale digital amplitude, order-two and order-three modulators
 *      may result in distorted or unstable behaviours and should be used
 *      with input signals up to about 75% of the full amplitude scale.
 *
 *      For best performance, 50% amplitude peaks should be used combined
 *      with oversampling to overcome the 6 dB loss in the signal-to-noise
 *      ratio.
 *
 *      Copyright (C) Dario Sanfilippo 2021 – sanfilippo.dario@gmail.com.
 *
 ******************************************************************************/

#include "dsm.h"

struct FullAdder {
    bool sum;
    bool c_out;
};

void dsm1(Sig* in, Sig* out) {
    assert(in->vec_len == out->vec_len);
    double state = 0.0;
    double integrator = 0.0;
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator += in->vec_space[0][i] - state;
        out->vec_space[0][i] = state = integrator < 0.0 ? -1.0 : 1.0;
    }
}

void dsm2(Sig* in, Sig* out) {
    assert(in->vec_len == out->vec_len);
    double state = 0.0;
    double integrator1 = 0.0;
    double integrator2 = 0.0;
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator1 += in->vec_space[0][i] - state;
        integrator2 += integrator1 - state * 2.0;
        out->vec_space[0][i] = state = integrator2 < 0.0 ? -1.0 : 1.0;
    }
}

void dsm3(Sig* in, Sig* out) {
    assert(in->vec_len == out->vec_len);
    double state = 0.0;
    double integrator1 = 0.0;
    double integrator2 = 0.0;
    double integrator3 = 0.0;
    double c1 = 1.0 / 4.0;
    double c2 = 1.0 / 3.0;
    double c3 = 1.0 / 8.0;
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator1 += in->vec_space[0][i] * c1 - state * c1;
        integrator2 += integrator1 * c2 - state * c2;
        integrator3 += integrator2 * c3 - state * c3;
        out->vec_space[0][i] = state = integrator3 < 0.0 ? -1.0 : 1.0;
    }
}

void fulladder(Sig* threeins, Sig* twoouts) {
    assert(threeins->vec_len == twoouts->vec_len);
    bool in0;
    bool in1;
    bool c_in;
    bool sum;
    bool c_out;
    for (size_t i = 0; i < threeins->vec_len; i++) {
        in0 = threeins->vec_space[0][i] > 0;
        in1 = threeins->vec_space[1][i] > 0;
        c_in = threeins->vec_space[2][i] > 0;
        sum = (in0 ^ in1) ^ c_in;
        c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
        twoouts->vec_space[0][i] = (double) sum == 0 ? -1.0 : 1.0;
        twoouts->vec_space[1][i] = (double) c_out == 0 ? -1.0 : 1.0;
    }
}

FullAdder fulladder_samplewise(bool in0, bool in1, bool c_in) {
    FullAdder fa;
    fa.sum = (in0 ^ in1) ^ c_in;
    fa.c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
    return fa;
}

void binaryadder(Sig* twoins, Sig* out) {
    assert(twoins->vec_len == out->vec_len);
    bool state = 0;
    bool in0;
    bool in1;
    bool c_in;
    bool sum;
    bool c_out;
    for (size_t i = 0; i < twoins->vec_len; i++) {
        in0 = twoins->vec_space[0][i] > 0;
        in1 = twoins->vec_space[1][i] > 0;
        c_in = state;
        sum = (in0 ^ in1) ^ c_in;
        c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
        state = sum;
        out->vec_space[0][i] = (double) c_out == 0 ? -1.0 : 1.0;
    }
}

bool binaryadder_samplewise(bool in0, bool in1, bool* state) {
    FullAdder fa = fulladder_samplewise(in0, in1, *state);
    *state = fa.sum;
    return fa.c_out;
}

void binarymultiplier(Sig* twoins, Sig* out) {
    assert(twoins->vec_len == out->vec_len);
    Sig** xorpair = malloc(sizeof(Sig*));
    Sig** sumpair = malloc(sizeof(Sig*));
    for (size_t i = 0; i < 8; i++) {
        sig_alloc(xorpair[i], 2, twoins->vec_len);
    }
    for (size_t i = 0; i < 7; i++) {
        sig_alloc(sumpair[i], 2, twoins->vec_len);
    }
    bool temp;
    /* the loops below compute the vectors corresponding to the 
     * 16 outputs of the xor operators */
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            for (size_t k = 3; k < twoins->vec_len; k++) {
                temp = (twoins->vec_space[0][k - i] > 0) ^ 
                    (twoins->vec_space[1][k - j] > 0);
                xorpair[j / 2 + i * 2]->vec_space[j % 2][k] = temp ? 1.0 : -1.0;
            }
        }
    }

    /* issues to solve: vec_space dimensions; signals need functions to
     * merge two spaces into a single one or split one into two smaller ones */

    /* this loop computes the eight summing units connected to the xor ops */
    for (size_t i = 0; i < 8; i++) {
        binaryadder(xorpair[i], sumpair[i / 2]);
    }
    /* this loop sums the previous summing ops two-by-two */
    for (size_t i = 0; i < 4; i++) {
        binaryadder(sum->vec_space + i * 2, sum->vec_space[8 + i], in->vec_len);
    }
    /* another summing stage */
    for (size_t i = 0; i < 2; i++) {
        binaryadder(sum->vec_space + 8 + i * 2, sum->vec_space[12 + i], in->vec_len);
    }
    /* last summing stage to compute the output */
    binaryadder(sum->vec_space + 12, out->vec_space[0], in->vec_len);
}
