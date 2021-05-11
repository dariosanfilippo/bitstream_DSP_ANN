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

void dsm1(double* in, double* out, size_t samples) {
    double state = 0.0;
    double integrator = 0.0;
    for (int i = 0; i < samples; i++) {
        integrator += in[i] - state;
        out[i] = state = integrator < 0.0 ? -1.0 : 1.0;
    }
}

void dsm2(double* in, double* out, size_t samples) {
    double state = 0.0;
    double integrator1 = 0.0;
    double integrator2 = 0.0;
    for (int i = 0; i < samples; i++) {
        integrator1 += in[i] - state;
        integrator2 += integrator1 - state * 2.0;
        out[i] = state = integrator2 < 0.0 ? -1.0 : 1.0;
    }
}

void dsm3(double* in, double* out, size_t samples) {
    double state = 0.0;
    double integrator1 = 0.0;
    double integrator2 = 0.0;
    double integrator3 = 0.0;
    double c1 = 1.0 / 4.0;
    double c2 = 1.0 / 3.0;
    double c3 = 1.0 / 8.0;
    for (int i = 0; i < samples; i++) {
        integrator1 += in[i] * c1 - state * c1;
        integrator2 += integrator1 * c2 - state * c2;
        integrator3 += integrator2 * c3 - state * c3;
        out[i] = state = integrator3 < 0.0 ? -1.0 : 1.0;
    }
}

void fulladder(double** ins, double** outs, size_t samples) {
    bool in0;
    bool in1;
    bool c_in;
    bool sum;
    bool c_out;
    for (int i = 0; i < samples; i++) {
        in0 = ins[0][i] > 0;
        in1 = ins[1][i] > 0;
        c_in = ins[2][i] > 0;
        sum = (in0 ^ in1) ^ c_in;
        c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
        outs[0][i] = (double) sum == 0 ? -1.0 : 1.0;
        outs[1][i] = (double) c_out == 0 ? -1.0 : 1.0;
    }
}

FullAdder fulladder_samplewise(bool in0, bool in1, bool c_in) {
    FullAdder fa;
    fa.sum = (in0 ^ in1) ^ c_in;
    fa.c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
    return fa;
}

void binaryadder(double** ins, double* out, size_t samples) {
    bool state = 0;
    bool in0;
    bool in1;
    bool c_in;
    bool sum;
    bool c_out;
    for (int i = 0; i < samples; i++) {
        in0 = ins[0][i] > 0;
        in1 = ins[1][i] > 0;
        c_in = state;
        sum = (in0 ^ in1) ^ c_in;
        c_out = (in0 & in1) | ((in0 ^ in1) & c_in);
        state = sum;
        out[i] = (double) c_out == 0 ? -1.0 : 1.0;
    }
}

bool binaryadder_samplewise(bool in0, bool in1, bool* state) {
    FullAdder fa = fulladder_samplewise(in0, in1, *state);
    *state = fa.sum;
    return fa.c_out;
}

void binarymultiplier(Sig* in, Sig* out) {
    Sig* xor = malloc(sizeof(Sig));
    Sig* sum = malloc(sizeof(Sig));
    sig_alloc(xor, 16, in->vec_len);
    sig_alloc(sum, 14, in->vec_len);
    bool temp;
    /* the loops below will computes the vectors corresponding to the 
     * 16 outputs of the xor operators */
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            for (size_t k = 3; k < in->vec_len; k++) {
                temp = (in->vec_space[0][k - i] > 0) ^ 
                    (in->vec_space[1][k - j] > 0);
                xor->vec_space[i * 4 + j][k] = temp ? 1.0 : -1.0;
            }
        }
    }
    /* this loop computes the eight summing units connected to the xor ops */
    for (size_t i = 0; i < 8; i++) {
        binaryadder(xor->vec_space + i * 2, sum->vec_space[i], in->vec_len);
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
