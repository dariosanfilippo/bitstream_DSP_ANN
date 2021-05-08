#include "auxiliary.h"

double** twod_alloc(size_t rows, size_t cols) {
    double** twod = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        twod[i] = malloc(cols * sizeof(double));
    }
    return twod;
}

void twod_free(double** ptr, size_t rows) {
    for (int i = 0; i < rows; i++) {
        free(ptr[i]);
    }
}
