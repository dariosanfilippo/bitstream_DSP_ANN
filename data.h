/*******************************************************************************
 *
 *      This module contains functions for the generation of training and
 *      test data for frequency and amplitude estimation, and sum and
 *      multiplication operations.
 *
 *      Copyright (c) Dario Sanfilippo 2021.
 *
*******************************************************************************/

#ifndef DATA
#define DATA

void freq_est_data(Sig* in, Sig* out);
void amp_est_data(Sig* in, Sig* out);
void adder_data(Sig* in, Sig* out);
void mul_data(Sig* in, Sig* out);

#endif
