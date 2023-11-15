#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "watershed_cuts.h"

double
clockdiff(struct timespec* end, struct timespec * start)
{
    double elapsed = (end->tv_sec - start->tv_sec);
    elapsed += (end->tv_nsec - start->tv_nsec) / 1000000000.0;
    return elapsed;
}


static void show_int_matrix(const int64_t * I, size_t M, size_t N)
{
    if(M*N > 100)
        return;

    for(size_t kk = 0; kk < M; kk++)
    {
        for(size_t ll = 0 ; ll < N; ll++)
        {
            printf("%ld ", I[kk + M*ll]);
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
    size_t P = 1;

    if(argc > 1)
    {
        M = atol(argv[1]);
        N = M;
        P = M;
    }


    printf("Input size: [%zu x %zu x %zu]\n", M, N, P);
    double * F = malloc(M*N*P*sizeof(double));
    assert(F != NULL);

    for(size_t kk = 0; kk<M*N*P; kk++)
    {
        F[kk] = 0; //(double) rand() / (double) RAND_MAX;
    }

    show_double_matrix(F, M, N);
    struct timespec tstart, tend;
    clock_gettime(CLOCK_REALTIME, &tstart);
    int64_t * W = watershed_cuts(F, M, N, P);
    clock_gettime(CLOCK_REALTIME, &tend);
    show_int_matrix(W, M, N);
    free(W);
    printf("Constant input, Took %f s\n", clockdiff(&tend, &tstart));

    srand(0);
    for(size_t kk = 0; kk<M*N*P; kk++)
    {
        F[kk] = (double) rand() / (double) RAND_MAX;
    }

    clock_gettime(CLOCK_REALTIME, &tstart);
    W = watershed_cuts(F, M, N, P);
    clock_gettime(CLOCK_REALTIME, &tend);
    free(F);
    show_int_matrix(W, M, N);
    free(W);
    printf("Random input, Took %f s\n", clockdiff(&tend, &tstart));

    return EXIT_SUCCESS;
}
