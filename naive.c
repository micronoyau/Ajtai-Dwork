#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "naive.h"
#include "linearalgebra.h"

const int C = 8;

void free_pubkey (struct PubKey *pubkey)
{
    for (int i=0; i<pubkey->n; i++)
    {
        free_vector (pubkey->w[i]);
    }
    for (int i=0; i< pow(pubkey->n,3); i++)
    {
        free_vector( pubkey->v[i] );
    }
    free (pubkey);
}

void init_random_seed ()
{
    srand(time(NULL));
}

void random_Bn (struct Vector *v)
{
    int n = v->dim;
    float rho_n = pow(n,n);
    for (int i=0; i<n; i++)
    {
        float r = ( (float) (rand() - RAND_MAX/2) ) / ((float)(RAND_MAX));
        v->coordinates[i] = rho_n * r;
    }
}

void random_Un (struct Vector *v)
{
    for (int i=0; i<v->dim; i++)
    {
        v->coordinates[i] = ( (float) (rand() - RAND_MAX/2) ) / ((float)(RAND_MAX/2));
    }
    div_vector ( v, norm(v), v );
    float norm = ((float) rand()) / ((float)RAND_MAX);
    mult_scalar_vector ( v, sqrt(norm), v );
}

void random_Sn (struct Vector *v)
{
    for (int i=0; i<v->dim; i++)
    {
        v->coordinates[i] = ( (float) (rand() - RAND_MAX/2) ) / ((float)(RAND_MAX/2));
    }
    div_vector ( v, norm(v), v );
    float norm = ((float) rand()) / (pow(((float)v->dim), C) * (float)RAND_MAX);
    mult_scalar_vector ( v, norm, v );
}

void random_Hu (struct Vector *u, struct Vector *res)
{
    int n = u->dim;
    float rho_n = pow(n,n);

    // Generating the part in a hyperplane parallel to u
    random_Bn (res);

    struct Vector *tmp;
    init_vector (&tmp, n);
    copy_vector (tmp, u);
    div_vector (tmp, pow(norm(u),2), tmp);
    float fractionnal_part = dot_product(res, u);
    fractionnal_part = fractionnal_part - (float) ((int)fractionnal_part);
    mult_scalar_vector ( tmp, fractionnal_part, tmp );
    sub_vector ( res, tmp, res );
    free_vector(tmp);

    // Adding small errors
    struct Vector *delta;
    init_vector(&delta,n);
    for (int i=0; i<n; i++)
    {
        random_Sn (delta);
        add_vector (res, delta, res);
    }
    free_vector(delta);
}


/*
############### Key Generation ##############
*/

void gen_pubkey (struct PubKey **pubkey, struct Vector *privkey)
{
    int n = privkey->dim;
    float rho_n = pow(n,n);
    *pubkey = malloc(sizeof(struct PubKey));
    (**pubkey).n = n;

    // W
    (**pubkey).w = malloc(sizeof(struct Vector*) * n);
    for (int i=0; i<n; i++)
    {
        init_vector(&(**pubkey).w[i], n);
    }

    while (width((**pubkey).w, n) < pow(n,-2) * rho_n)
    {
        for (int i=0; i<n; i++)
        {
            random_Hu (privkey, (**pubkey).w[i]);
        }
    }

    // V
    (**pubkey).v = malloc(sizeof(struct Vector) * n*n*n);
    for (int i=0; i<n*n*n; i++)
    {
        init_vector (&(**pubkey).v[i], n);
        random_Hu (privkey, (**pubkey).v[i]);
    }
}

void gen_privkey (struct Vector **privkey, int n)
{
    init_vector (privkey, n);
    random_Un (*privkey);
}


/*
################ Encryption / Decryption primitives ##############
*/

void encrypt_bit (struct Vector *res, struct PubKey *pubkey, char bit)
{
    if (bit==0)
    {
        // Select b_i in {0,1}
        for (int i=0; i<pow(pubkey->n, 3); i++)
        {
            int rand = (int) ((float)random() / ((float)RAND_MAX) + 0.5);
            if (rand == 1)
            {
                add_vector (res, pubkey->v[i], res);
                mod_vector (res, pubkey->w, res);
            }
        }
    } else {
        random_Bn(res);
    }
}

char decrypt_bit (struct Vector *privkey, struct Vector *cipher)
{
    float n = (float)privkey->dim;
    float tau = dot_product (privkey, cipher);
    tau = tau - (float)((int)tau);

    if ( ((-1 <= tau) && (tau <= -1+1/n))
      || ((-1/n <= tau) && (tau <= 1/n))
      || ((1-1/n <= tau) && (tau <= 1)) )
    {
        return 0;
    }
    return 1;
}
