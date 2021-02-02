#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "linearalgebra.h"

/*
################## Vector operations ##############
*/

void init_vector (struct Vector **v, int dim)
{
    *v = malloc(sizeof(struct Vector));
    (**v).dim = dim;
    (**v).coordinates = malloc(sizeof(float)*dim);
    zero_vector(*v);
}

void free_vector (struct Vector *v)
{
    // Making sure to wipe data after usage
    zero_vector (v);
    free (v->coordinates);
    free(v);
}

void zero_vector (struct Vector *v)
{
    for (int i=0; i<v->dim; i++)
    {
        v->coordinates[i] = 0;
    }
}

void copy_vector (struct Vector *dst, struct Vector *src)
{
    for (int i=0; i<src->dim; i++)
    {
        dst->coordinates[i] = src->coordinates[i];
    }
}

void display_vector (struct Vector *v)
{
    for (int i=0; i<v->dim; i++)
    {
        printf("%f  ", v->coordinates[i]);
    }
    printf("\n");
}

float dot_product (struct Vector *v1, struct Vector *v2)
{
    if (v1->dim != v2->dim)
        return 0;

    float ret = 0;
    for (int i=0; i<v1->dim; i++)
    {
        ret += v1->coordinates[i] * v2->coordinates[i];
    }
    return ret;
}

float norm (struct Vector *v)
{
    return sqrt(dot_product(v,v));
}

void mult_scalar_vector (struct Vector *v, float scalar, struct Vector *res)
{
    for (int i=0; i<v->dim; i++)
    {
        res->coordinates[i] = scalar * v->coordinates[i];
    }
}

void div_vector (struct Vector *v, float scalar, struct Vector *res)
{
    mult_scalar_vector (v, 1/scalar, res);
}

void add_vector (struct Vector *v1, struct Vector *v2, struct Vector *res)
{
    for (int i=0; i<v1->dim; i++)
    {
        res->coordinates[i] = v1->coordinates[i] + v2->coordinates[i];
    }
}

void sub_vector (struct Vector *v1, struct Vector *v2, struct Vector *res)
{
    for (int i=0; i<res->dim; i++)
    {
        res->coordinates[i] = v1->coordinates[i] - v2->coordinates[i];
    }
}

void mod_vector (struct Vector *v, struct Vector **parallelepiped, struct Vector *res)
{
    /*
     * Computes [v] (mod [parallellepiped]).
     * The way we proceed here is by rounding coordinates in the canonical basis.
     */
    struct Vector *tmp;
    struct Vector *tmp2;
    init_vector(&tmp, v->dim);
    init_vector(&tmp2, v->dim);

    // Matrix from canonical basis to paralleliped basis
    struct Matrix *P;
    init_matrix (&P, v->dim, v->dim);
    for (int i=0; i<P->p; i++)
    {
        for (int j=0; j<P->q; j++)
        {
            P->coefficients[i][j] = parallelepiped[j]->coordinates[i];
        }
    }

    struct Matrix *inv;
    init_matrix (&inv, P->p, P->q);
    inv_matrix (P, inv);

    mult_matrix_vector (inv, v, tmp);

    for (int i=0; i<tmp->dim; i++)
    {
        tmp->coordinates[i] = roundf (tmp->coordinates[i]);
    }

    mult_matrix_vector (P, tmp, tmp2);

    sub_vector (v, tmp2, res);

    free_vector (tmp);
    free_vector (tmp2);
    free_matrix (P);
}

void sub_mod (struct Vector *v1, struct Vector *v2, struct Vector **parallelepiped, struct Vector *res)
{
    /*
     * Computes [v1] - [v2] (mod [parallellepiped]).
     */
    sub_vector (v1, v2, res);
    mod_vector (res, parallelepiped, res);
}

void gram_schmidt (struct Vector **collection, struct Vector **orthogonalized, int length)
{
    struct Vector *tmp;
    init_vector(&tmp, collection[0]->dim);

    for (int i=0; i<length; i++)
    {
        copy_vector ( orthogonalized[i], collection[i] );
        for (int j=0; j<i; j++)
        {
            if (norm(orthogonalized[j]) != 0)
                mult_scalar_vector ( orthogonalized[j], dot_product(collection[i], orthogonalized[j]) / pow( norm(orthogonalized[j]), 2 ), tmp );
            sub_vector ( orthogonalized[i], tmp, orthogonalized[i] );
        }
        zero_vector (tmp);
    }

    free_vector(tmp);
}

float width (struct Vector **collection, int length)
{
    struct Vector **orthogonalized = malloc(sizeof(struct Vector*) * length);
    for (int i=0; i<length; i++)
    {
        init_vector(&orthogonalized[i], collection[0]->dim);
    }

    gram_schmidt (collection, orthogonalized, length);

    float min = norm(orthogonalized[0]);
    for (int i=1; i<length; i++)
    {
        float norm_ = norm(orthogonalized[i]);
        if (min > norm_)
            min = norm_;
        free_vector (orthogonalized[i]);
    }

    return min;
}

/*
################ Matrix operations ##################
*/

void init_matrix (struct Matrix **m, int p, int q)
{
    *m = malloc ( sizeof(struct Matrix) );
    (**m).p = p;
    (**m).q = q;
    (**m).coefficients = malloc ( sizeof(float*) * p );
    for (int i=0; i<p; i++)
    {
        (**m).coefficients[i] = malloc (sizeof(float)*q);
        for (int j=0; j<q; j++)
        {
            (**m).coefficients[i][j] = 0;
        }
    }
}

void free_matrix (struct Matrix *m)
{
    // Wiping data after usage
    zero_matrix (m);
    for (int i=0; i<m->p; i++)
    {
        free (m->coefficients[i]);
    }
    free (m->coefficients);
    free (m);
}

void id_matrix (struct Matrix *m)
{
    for (int i=0; i<m->p; i++)
    {
        for (int j=0; j<m->q; j++)
        {
            m->coefficients[i][j] = 0;
        }
        m->coefficients[i][i] = 1;
    }
}

void zero_matrix (struct Matrix *m)
{
    for (int i=0; i<m->p; i++)
    {
        for (int j=0; j<m->q; j++)
        {
            m->coefficients[i][j] = 0;
        }
    }
}

void copy_matrix (struct Matrix *dst, struct Matrix *src)
{
    for (int i=0; i<dst->p; i++)
    {
        for (int j=0; j<dst->q; j++)
        {
            dst->coefficients[i][j] = src->coefficients[i][j];
        }
    }
}

void display_matrix (struct Matrix *m)
{
    for (int i=0; i<m->p; i++)
    {
        for (int j=0; j<m->q; j++)
        {
            printf("%f  ", m->coefficients[i][j]);
        }
        printf("\n");
    }
}

void mult_matrix_vector (struct Matrix *m, struct Vector *v, struct Vector *out)
{
    if ( m->q != v->dim || m->p != out->dim )
    {
        printf("Vector and matrix do not have compatible sizes\n");
        exit(1);
    }

    for (int i=0; i<m->p; i++)
    {
        out->coordinates[i] = 0;
        for (int j=0; j<m->q; j++)
        {
            out->coordinates[i] += m->coefficients[i][j] * v->coordinates[j];
        }
    }
}

void div_line (struct Matrix *m, int line, float scalar)
{
    for (int j=0; j<m->q; j++)
    {
        m->coefficients[line][j] /= scalar;
    }
}

void inv_matrix (struct Matrix *m_, struct Matrix *inv)
{
    /*
     * Gauss pivot method.
     */
    struct Matrix *m;
    init_matrix (&m, m_->p, m_->q);
    copy_matrix (m, m_);

    if ( m->q != m->p )
    {
        printf("Trying to invert a non-square matrix ...\n");
    }

    id_matrix (inv);

    // Triangular matrix
    for (int i=0; i<m->p; i++)
    {
        div_line (inv, i, m->coefficients[i][i]);
        div_line (m, i, m->coefficients[i][i]);

        for (int k=i+1; k<m->p; k++)
        {
            for (int j=i+1; j<m->q; j++)
            {
                m->coefficients[k][j] -= m->coefficients[k][i] * m->coefficients[i][j];
            }
            for (int j=0; j<m->q; j++)
            {
                inv->coefficients[k][j] -= m->coefficients[k][i] * inv->coefficients[i][j];
            }
            m->coefficients[k][i] = 0;
        }
    }

    for (int i=m->p-1; i>=0; i--)
    {
        for (int j=0; j<i; j++)
        {
            for (int k=0; k<m->q; k++)
            {
                inv->coefficients[j][k] -= m->coefficients[j][i] * inv->coefficients[i][k];
            }
        }
    }

    free_matrix(m);
}
