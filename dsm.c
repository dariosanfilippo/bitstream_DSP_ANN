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

//void binarymultiplier(double** ins, double* out, size_t samples) {


















