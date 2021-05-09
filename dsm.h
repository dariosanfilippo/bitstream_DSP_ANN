/* 
 * *****************************************************************************
 *
 *      Delta-sigma modulators and arithmetic.
 *
 * *****************************************************************************
 */

#ifndef DSM
#define DSM

typedef struct FullAdder FullAdder;

void dsm1(double* in, double* out, size_t samples);
void dsm2(double* in, double* out, size_t samples);
void dsm3(double* in, double* out, size_t samples);
void fulladder(double** in, double** out, size_t samples);
void binaryadder(double** in, double* out, size_t samples);
void binarymultiplier(double** in, double* out, size_t samples);

#endif
