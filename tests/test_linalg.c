#include <stdio.h>
#include <stdlib.h>
#include "linearalgebra.h"

void fail(char* str) {
    printf("[test_linalg] : ERROR : %s\n", str);
    exit(-1);
}

int unit_create_init_vector() {
    struct Vector *v = NULL;

    init_vector(&v, 3);
    if (v == NULL)
        fail("Unable to allocate memory for vector");

    if (v->dim != 3)
        fail("Wrong vector dimension after init");

    v->coordinates[0] = 1.0;
    v->coordinates[1] = 2.0;
    v->coordinates[2] = 3.0;

    free_vector(v);

    return 0;
}

int unit_zero_vector() {
    struct Vector *v = NULL;

    init_vector(&v, 3);

    v->coordinates[0] = 1.0;
    v->coordinates[1] = 2.0;
    v->coordinates[2] = 3.0;

    zero_vector(v);

    if (v->coordinates[0] != 0.0 || v->coordinates[1] != 0.0 || v->coordinates[2] != 0.0)
        fail("Zero failed");

    free_vector(v);

    return 0;
}

int unit_copy_vector() {
    struct Vector *u = NULL;
    struct Vector *v = NULL;

    init_vector(&u, 3);
    init_vector(&v, 3);

    u->coordinates[0] = 1.0;
    u->coordinates[1] = 2.0;
    u->coordinates[2] = 3.0;

    copy_vector(v, u);

    if (v->coordinates[0] != 1.0 || v->coordinates[1] != 2.0 || v->coordinates[2] != 3.0)
        fail("Copy failed");

    free_vector(v);

    return 0;
}

int unit_display_vector() {
    struct Vector *v = NULL;

    init_vector(&v, 3);

    v->coordinates[0] = 1.0;
    v->coordinates[1] = 2.0;
    v->coordinates[2] = 3.0;

    display_vector(v);

    free_vector(v);

    return 0;
}

int unit_dot_product() {
    struct Vector *u = NULL;
    struct Vector *v = NULL;

    init_vector(&u, 3);
    init_vector(&v, 3);

    u->coordinates[0] = 4.0;
    u->coordinates[1] = 5.0;
    u->coordinates[2] = 6.0;

    v->coordinates[0] = 1.0;
    v->coordinates[1] = -2.0;
    v->coordinates[2] = 3.0;

    if (dot_product(u,v) != 12.0)
        fail("Wrong result for dot product");

    if (dot_product(v,u) != 12.0)
        fail("Wrong result for dot product (commutation not respected)");

    free_vector(v);

    return 0;
}

// Summary of all tests to do
const int (*TESTS[]) (void) = {
    unit_create_init_vector,
    unit_zero_vector,
    unit_copy_vector,
    unit_display_vector,
    unit_dot_product
};

const int N_TESTS = sizeof(TESTS) / sizeof(int (*)(void));


int main(void) {
    for (int i=0; i<N_TESTS; i++) {
        TESTS[i]();
        printf("[+] Test %d/%d done\n", i+1, N_TESTS);
    }
    return 0;
}

