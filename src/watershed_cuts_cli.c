#include <stdio.h>
#include "watershed_cuts.h"

static void show_double_matrix(const double * I, size_t M, size_t N)
{
    for(size_t ll = 0 ; ll < N; ll++)
    {
        for(size_t kk = 0; kk < M; kk++)
        {
            printf("% .1f ", I[kk + M*ll]);
        }
        printf("\n");
    }
}

static void show_int_matrix(const int * I, size_t M, size_t N)
{
    for(size_t ll = 0 ; ll < N; ll++)
    {
        for(size_t kk = 0; kk < M; kk++)
        {
            printf("%d ", I[kk + M*ll]);
        }
        printf("\n");
    }
}


int main(int argc, char ** argv)
{
    if(argc > 1)
    {
        printf("%s\n", argv[0]);
    }

    size_t M = 5;
    size_t N = 5;
    double * F = malloc(M*N*sizeof(double));
    for(size_t kk = 0; kk<M*N; kk++)
    {
        F[kk] = 10; //(double) rand() / (double) RAND_MAX;
    }

    F[12] = 1;
    F[13] = 1;

    show_double_matrix(F, M, N);
    int * W = watershed_cuts(F, M, N);
    show_int_matrix(W, M, N);

    free(W);
    return EXIT_SUCCESS;
}
