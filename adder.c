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
    size_t inputs = 64;
    size_t outputs = inputs / 2;
    size_t num_layers = 1;
    size_t neurons = outputs;
    size_t SR = 192000;

    /* Create the neural network */
    t = kann_layer_input(inputs);
    for (size_t i = 0; i < num_layers; i++) {
        t = kann_layer_dense(t, neurons);
        t = kad_tanh(t);
    }
    t = kann_layer_cost(t, outputs, KANN_C_MSE);
    ann = kann_new(t, 0);

    /* Create training data set */
    size_t setsize = 65536;
    Sig* x = malloc(sizeof(Sig));
    Sig* y = malloc(sizeof(Sig));
    sig_alloc(x, setsize, inputs, SR);
    sig_alloc(y, setsize, outputs, SR);
    adder_data(x, y);

    /* Train the net */
    float lr = .001, frac_val = .1;
    size_t mini_size = 32;
    size_t max_epoch = 1000;
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
    fprintf(csv, "%s\n", "Error rate");

    /* Test the net */
    size_t testsize = 100;
    size_t outlen = 4096;
    size_t err_count = 0;
    audio err_rate = 0;
    x = malloc(sizeof(Sig));
    y = malloc(sizeof(Sig));
    const float* output = malloc(sizeof(float) * outlen);
    float* rectified = malloc(sizeof(float) * outlen);

    sig_alloc(x, testsize, outlen * 2, SR);
    sig_alloc(y, testsize, outlen, SR);
    adder_data(x, y);

    for (size_t j = 0; j < testsize; j++) {
        output = kann_apply1(ann, x->vec_space[j]);
        for (size_t i = 0; i < y->vec_len; i++) {
            rectified[i] = output[i] < .0 ? -1.0 : 1.0;
            printf("Target: %.10f; prediction: %.10f; error factor: %.10f\n", 
                y->vec_space[j][i], rectified[i], y->vec_space[j][i] / rectified[i]);
            err_count = y->vec_space[j][i] / rectified[i] < 0 ? err_count + 1 : 
                err_count;
        }
        err_rate = err_count / (audio) y->vec_len;
        fprintf(csv, "%f\n", err_rate);
        err_count = 0;
    }
    
    sig_free(x);
    sig_free(y);
    free((float*) output);
    free(rectified);
    fclose(csv);

    return EXIT_SUCCESS;
}
