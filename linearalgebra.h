#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

struct Vector
{
    int dim;
    float *coordinates;
};

struct Matrix
{
    int p; // Number of rows
    int q; // Number of columns
    float **coefficients;
};

/*
########### Vector operations ###########
*/

void init_vector (struct Vector **v, int dim); // Initialize to zero vector
void free_vector (struct Vector *v);
void zero_vector (struct Vector *v);
void copy_vector (struct Vector *dst, struct Vector *src);
void display_vector (struct Vector *v); // Display in row-style format

float dot_product (struct Vector *v1, struct Vector *v2);
float norm (struct Vector *v);

void mult_scalar_vector (struct Vector *v, float scalar, struct Vector *res);
void div_vector (struct Vector *v, float scalar, struct Vector *res);
void add_vector (struct Vector *v1, struct Vector *v2, struct Vector *res);
void sub_vector (struct Vector *v1, struct Vector *v2, struct Vector *res);
void mod_vector (struct Vector *v, struct Vector **parallelepiped, struct Vector *res);
void sub_mod (struct Vector *v1, struct Vector *v2, struct Vector **parallelepiped, struct Vector *res); // Substract modulo a parallelogram centered in 0

// Generate orthogonal collection of vectors with gram schmidt method
void gram_schmidt (struct Vector **collection, struct Vector **orthogonalized, int length);
// Get the width of a parallelepiped
float width (struct Vector **collection, int length);

/*
########### Matrix operations ###########
*/

void init_matrix (struct Matrix **m, int p, int q); // Initialize a zero matrix
void free_matrix (struct Matrix *m);
void zero_matrix (struct Matrix *m);
void id_matrix (struct Matrix *m);
void copy_matrix (struct Matrix *dst, struct Matrix *src);
void display_matrix (struct Matrix *m);

void mult_matrix_vector (struct Matrix *m, struct Vector *v, struct Vector *out);
void inv_matrix (struct Matrix *m, struct Matrix *inv);

#endif
