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

void dsm1(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id) {
    assert(in->vec_len == out->vec_len);
    double state = 0.0;
    double integrator = 0.0;
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator += in->vec_space[in_vec_id][i] - state;
        out->vec_space[out_vec_id][i] = state = integrator < 0.0 ? -1.0 : 1.0;
    }
}

void dsm2(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id) {
    assert(in->vec_len == out->vec_len);
    double state = 0.0;
    double integrator1 = 0.0;
    double integrator2 = 0.0;
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator1 += in->vec_space[in_vec_id][i] - state;
        integrator2 += integrator1 - state * 2.0;
        out->vec_space[out_vec_id][i] = state = integrator2 < 0.0 ? -1.0 : 1.0;
    }
}

void dsm3(Sig* in, Sig* out, size_t in_vec_id, size_t out_vec_id) {
    assert(in->vec_len == out->vec_len);
    double state = 0.0;
    double integrator1 = 0.0;
    double integrator2 = 0.0;
    double integrator3 = 0.0;
    double c1 = 1.0 / 4.0;
    double c2 = 1.0 / 3.0;
    double c3 = 1.0 / 8.0;
    for (size_t i = 0; i < in->vec_len; i++) {
        integrator1 += in->vec_space[in_vec_id][i] * c1 - state * c1;
        integrator2 += integrator1 * c2 - state * c2;
        integrator3 += integrator2 * c3 - state * c3;
        out->vec_space[out_vec_id][i] = state = integrator3 < 0.0 ? -1.0 : 1.0;
    }
}

void fulladder(Sig* in0, Sig* in1, Sig* in2, Sig* out0, Sig* out1, 
    size_t in_vec_id0, size_t in_vec_id1, size_t in_vec_id2, size_t out_vec_id0, 
        size_t out_vec_id1) {
    assert((in0->vec_len == in1->vec_len) & (in1->vec_len == in2->vec_len) & 
        (in2->vec_len == out0->vec_len) & (out0->vec_len == out1->vec_len));
    bool _in0;
    bool _in1;
    bool c_in;
    bool sum;
    bool c_out;
    for (size_t i = 0; i < in0->vec_len; i++) {
        _in0 = in0->vec_space[in_vec_id0][i] > 0;
        _in1 = in1->vec_space[in_vec_id1][i] > 0;
        c_in = in2->vec_space[in_vec_id2][i] > 0;
        sum = (_in0 ^ _in1) ^ c_in;
        c_out = (_in0 & _in1) | ((_in0 ^ _in1) & c_in);
        out0->vec_space[out_vec_id0][i] = (double) sum == 0 ? -1.0 : 1.0;
        out1->vec_space[out_vec_id1][i] = (double) c_out == 0 ? -1.0 : 1.0;
    }
}

FullAdder fulladder_samplewise(bool in0, bool in1, bool c_in) {
    FullAdder fa;
    fa.sum = (in0 ^ in1) ^ c_in;
    fa.c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
    return fa;
}

void binaryadder(Sig* in0, Sig* in1, Sig* out, size_t in_vec_id0, size_t in_vec_id1, 
    size_t out_vec_id) {
    assert((in0->vec_len == in1->vec_len) & (in1->vec_len == out->vec_len));
    bool state = 0;
    bool _in0;
    bool _in1;
    bool c_in;
    bool sum;
    bool c_out;
    for (size_t i = 0; i < in0->vec_len; i++) {
        _in0 = in0->vec_space[in_vec_id0][i] > 0;
        _in1 = in1->vec_space[in_vec_id1][i] > 0;
        c_in = state;
        sum = (_in0 ^ _in1) ^ c_in;
        c_out = (_in0 & _in1) | ((_in0 ^ _in1) & c_in);
        state = sum;
        out->vec_space[out_vec_id][i] = (double) c_out == 0 ? -1.0 : 1.0;
    }
}

bool binaryadder_samplewise(bool in0, bool in1, bool* state) {
    FullAdder fa = fulladder_samplewise(in0, in1, *state);
    *state = fa.sum;
    return fa.c_out;
}

void binarymultiplier(Sig* in0, Sig* in1, Sig* out, size_t in_vec_id0, 
    size_t in_vec_id1, size_t out_vec_id) {
    assert((in0->vec_len == in1->vec_len) & (in1->vec_len == out->vec_len));
    Sig* xor = malloc(sizeof(Sig));
    Sig* sum = malloc(sizeof(Sig));
    sig_alloc(xor, 16, in0->vec_len);
    sig_alloc(sum, 14, in0->vec_len);
    bool temp;
    /* the loops below compute the vectors corresponding to the 
     * 16 outputs of the xor operators */
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            for (size_t k = 3; k < in0->vec_len; k++) {
                temp = (in0->vec_space[in_vec_id0][k - i] > 0) ^ 
                    (in1->vec_space[in_vec_id1][k - j] > 0);
                xor->vec_space[j + i * 4][k] = temp ? 1.0 : -1.0;
            }
        }
    }
    for (size_t i = 0; i < 8; i++) {
        binaryadder(xor, xor, sum, i * 2, i * 2 + 1, i);
    }
    for (size_t i = 0; i < 4; i++) {
        binaryadder(sum, sum, sum, i * 2, i * 2 + 1, i + 8);
    }
    for (size_t i = 0; i < 2; i++) {
        binaryadder(sum, sum, sum, i * 2 + 8, i * 2 + 8 + 1, i + 12);
    }
    binaryadder(sum, sum, out, 12, 13, out_vec_id);
}
