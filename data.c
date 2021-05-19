void gen_freq(Sig* in, Sig* out) {
    double freq = 0;
    for (size_t i = 0; i < in->space_dim; i++) {
        freq = (rand() / (double) RAND_MAX) * 800 + 200;
        sine(192000, .5, freq, in->vec_space[i], in->vec_len);
        dsm3(in->vec_space[i], in->vec_space[i], in->vec_len);
        out->vec_space[i][0] = freq;
    }
}

