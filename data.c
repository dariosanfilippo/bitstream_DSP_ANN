void gen_freq(Sig* in) {
    double freq = 0;
    for (size_t i = 0; i < in->space_dim; i++) {
        freq = (rand() / (double) RAND_MAX) * 1000;
        sine(192000, .5, freq, in->vec_space[i], in->vec_len);
        dsm3(in->vec_space[i], in->vec_space[i], in->vec_len);
    }
}

