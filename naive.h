#ifndef NAIVE_H
#define NAIVE_H

struct PubKey
{
    int n;
    struct Vector **v; // Array of vectors
    struct Vector **w; // Array of vectors
    int i1;
};

void free_pubkey (struct PubKey *pubkey);

void init_random_seed (); // Pick up random vectors with uniform distribution
void random_Bn (struct Vector *v); // Bn is the surrounding space (big hypercube of size rho_n)
void random_Un (struct Vector *v); // Un is the unit ball
void random_Sn (struct Vector *v); // Sn is a small ball of radius n^-c (to add errors to vectors)
void random_Hu (struct Vector *u, struct Vector *res); // Hu is the set of hyperplanes orthogonal to u, shifted by a multiple of u

/*
 * Key generation
 */

void gen_pubkey (struct PubKey **pubkey, struct Vector *privkey);
void gen_privkey (struct Vector **privkey, int n);

/*
 * Encryption / Decryption primitives
 */

void encrypt_bit (struct Vector *res, struct PubKey *pubkey, char bit);
char decrypt_bit (struct Vector *privkey, struct Vector *cipher);

#endif
