#include <stdlib.h>
#include <stdio.h>
#include "crypto.h"
#include "linearalgebra.h"
#include <math.h>

int main ()
{
    int n = 4;
    init_random_seed();

    struct Vector *privkey;
    gen_privkey (&privkey, n);

    struct PubKey *pubkey;
    gen_pubkey(&pubkey, privkey);

    printf("W = \n");
    for (int i=0; i<n; i++)
    {
        display_vector (pubkey->w[i]);
    }


    printf("V = \n");
    for (int i=0; i<pow(n,3); i++)
    {
        display_vector (pubkey->v[i]);
    }

    struct Vector *encrypted;
    init_vector (&encrypted, n);
    encrypt_bit (encrypted, pubkey, 1);

    printf("Encrypted : \n");
    display_vector (encrypted);

    printf("Decrypted : %d\n", decrypt_bit(privkey, encrypted));

    return 0;
}
