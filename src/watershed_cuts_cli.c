#include <stdio.h>
#include "watershed_cuts.h"


static void show_int_matrix(const int * I, size_t M, size_t N)
{
    if(M*N > 100)
        return;

    for(size_t kk = 0; kk < M; kk++)
    {
        for(size_t ll = 0 ; ll < N; ll++)
        {
            printf("%d ", I[kk + M*ll]);
        }
        printf("\n");
    }
}

static void show_double_matrix(const double * I, size_t M, size_t N)
{
    if(M*N > 100)
        return;


    for(size_t kk = 0; kk < M; kk++)
    {
        for(size_t ll = 0 ; ll < N; ll++)
        {
            printf("% .1f ", I[kk + M*ll]);
        }
        printf("\n");
    }
}




int main(int argc, char ** argv)
{
    size_t M = 5;
    size_t N = 5;

    if(argc > 1)
    {
        M = atol(argv[1]);
        N = M;
    }


    double * F = malloc(M*N*sizeof(double));
    for(size_t kk = 0; kk<M*N; kk++)
    {
        F[kk] = 7; //(double) rand() / (double) RAND_MAX;
    }

    if(0){
    F[1] = 0;
    F[12] = 0;
    F[13] = 0;
    }

    show_double_matrix(F, M, N);
    int * W = watershed_cuts(F, M, N);
    show_int_matrix(W, M, N);

    free(W);
    return EXIT_SUCCESS;
}
