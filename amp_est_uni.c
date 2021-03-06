/*******************************************************************************
 *
 *      This file is used to train and test unipolar networks for the estimation
 *      of the peak amplitude in DSM sinusoids with different frequency, 
 *      amplitude, and phase values. These networks, in particular, 
 *      are used with the ReLU and sigmoid activation functions.
 *
 *      Copyright (c) Dario Sanfilippo 2021
 *
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "signals.c"
#include "random.c"
#include "osc.c"
#include "dsm.c"
#include "bitmath.c"
#include "data.c"
#include "kann.c"
#include "kautodiff.c"

int main(void) {

    srand(time(0));

    kad_node_t* t;
    kann_t* ann;
    size_t inputs = 1024;
    size_t outputs = 1;
    size_t num_layers = 4;
    size_t neurons = 64;
    size_t SR = 192000;

    /* Create the neural network */
    t = kann_layer_input(inputs);
    for (size_t i = 0; i < num_layers; i++) {
        t = kann_layer_dense(t, neurons);
        t = kad_sigm(t);
    }
    t = kann_layer_cost(t, outputs, KANN_C_MSE);
    ann = kann_new(t, 0);

    /* Create training data set */
    size_t setsize = 65536;
    Sig* x = malloc(sizeof(Sig));
    Sig* y = malloc(sizeof(Sig));
    sig_alloc(x, setsize, inputs, SR);
    sig_alloc(y, setsize, outputs, SR);
    
    amp_est_data(x, y);
    for (size_t i = 0; i < setsize; i++) {
        bi2uni(x, x, i, i);
    }

    /* Train the net */
    float lr = .001, frac_val = .1;
    size_t mini_size = 32;
    size_t max_epoch = 500;
    size_t max_drop_streak = 20;
    kann_train_fnn1(ann, lr, mini_size, max_epoch, max_drop_streak, frac_val,
        setsize, x->vec_space, y->vec_space);
    sig_free(x);
    sig_free(y);

    /* CSV file setup */
    char fname[256];
    sprintf(fname, "inputs%zuoutputs%zulayers%zuneurons%zuset%zulearningrate%f.csv", 
        inputs, outputs, num_layers, neurons, setsize, lr);
    FILE* csv;
    csv = fopen(fname, "w+");

    /* Test the net */
    size_t testsize = 100;
    x = malloc(sizeof(Sig));
    y = malloc(sizeof(Sig));
    sig_alloc(x, testsize, inputs, SR);
    sig_alloc(y, testsize, outputs, SR);
    const float* output;
    
    amp_est_data(x, y);
    for (size_t i = 0; i < testsize; i++) {
        bi2uni(x, x, i, i);
    }
    for (size_t j = 0; j < testsize; j++) {
        output = kann_apply1(ann, x->vec_space[j]);
        printf("Target: %.10f; prediction: %.10f; error difference: %.10f\n", 
            y->vec_space[j][0], y->vec_space[j][0], y->vec_space[j][0] - *output);
        fprintf(csv, "%f, %f\n", y->vec_space[j][0], y->vec_space[j][0] - *output);
    }
    sig_free(x);
    sig_free(y);
    fclose(csv);

    return EXIT_SUCCESS;
}
