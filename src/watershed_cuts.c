#include "watershed_cuts.h"

#include <stdio.h>

#define MIN(a,b)                                \
    ({ __typeof__ (a) _a = (a);                 \
        __typeof__ (b) _b = (b);                \
        _a < _b ? _a : _b; })


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


/* Return smallest value in the neighborhood of idx
* TODO XXX this is probably not what they describe in the paper.
* check p. 4 and make sure that you understand the example in Fig. 3
*/
static double * compute_F0(const double * restrict F,
                           const size_t M, const size_t N)
{
    double * F0 = malloc(M*N*sizeof(double));
    for(size_t ll = 0; ll < N; ll++)
    {
        F0[M*ll] = MIN(F[ll*M], F[1 + ll*M]);

        for(size_t kk = 1; kk+1 < M; kk++)
        {
            F0[kk+M*ll] = MIN(
                              MIN(F[kk-1 + ll*M], F[kk + ll*M]),
                              F[kk+1 + ll*M]);
        }
        F0[M-1 + M*ll] = MIN(F[M-2 + ll*M], F[M-1 + ll*M]);
    }

    {
        size_t ll = 0;
        for(size_t kk = 0; kk < M; kk++)
        {
            F0[kk+M*ll] = MIN(F0[kk+ll*M], F[kk+(ll+1)*M]);
        }
        ll = N-1;
        for(size_t kk = 0; kk < M; kk++)
        {
            F0[kk+M*ll] = MIN(F0[kk+ll*M], F[kk+(ll-1)*M]);
        }
    }

    for(size_t ll = 1; ll+1 < N; ll++)
    {

        for(size_t kk = 0; kk < M; kk++)
        {
            F0[kk+M*ll] = MIN(
                              MIN(F[kk + (ll-1)*M], F0[kk+ll*M]),
                              F[kk+(ll+1)*M]);
        }
    }
    return F0;
}

#define NO_LABEL 0

static int find_neighbour(const size_t y, size_t * restrict _z,
                          const int * restrict P,
                          const double * restrict F, const double * restrict F0,
                          const size_t M, const size_t N)
{
    size_t m = y % M;
    size_t n = (y-m) / M;
    //printf("Any neighbor to %zu? = (%zu, %zu)\n", y, m, n);
    if(m > 0)
    {
        size_t z = y-1;
        if( F[z] == F0[y] && F[y] == F0[y])
        {
            if( P[z] != -1 )
            {
                *_z = z;
                return 1;
            }
        }
    }

    if(m+1 < M)
    {
        size_t z = y+1;
        if( F[z] == F0[y] && F[y] == F0[y])
        {
            if(P[z] != -1)
            {
                *_z = z;
                return 1;
            }
        }
    }

    if(n > 0)
    {
        size_t z = y - M;
        if( F[z] == F0[y] && F[y] == F0[y])
        {
            if( P[z] != -1 )
            {
                *_z = z;
                return 1;
            }
        }
    }

    if(n+1 < N)
    {
        size_t z = y + M;
        if( F[z] == F0[y] && F[y] == F0[y])
        {
            if( P[z] != -1 )
            {
                *_z = z;
                return 1;
            }
        }
    }

    return 0;
}

static int get_stream(const size_t x,
                      const double * restrict F,
                      const double * restrict Fo,
                      int * restrict P,
                      const size_t M, const size_t N,
                      size_t * restrict L, size_t * restrict Lp, size_t * _nL)
{
    // x is the start pixel
    // Lp is s stack for pixels to visit
    // L is a stack for the pixels in the stream

    size_t nL = 0; // Empty stream
    size_t nLp = 0;

    L[nL++] = x;
    P[x] = -1; // temporary label
    Lp[nLp++] = x;

    while(nLp > 0)
    {
        size_t y = Lp[--nLp]; // pop

        // z is neighbour to y such that F({y,z}) == Fo(y)
        // and z not in L
        size_t z = 0;
        int breadth_first = 1;
        while(breadth_first && find_neighbour(y, &z, P, F, Fo, M, N) )
        {
           if(P[z] == -1)
           {
               printf("P[z] == -1!\n");
           }
            //printf("z= %zu\n", z);
            if(P[z] > 0) // if already has a label
            {
                /* Merge with existing label */
                *_nL = nL;
                //printf("Found label %d (nL=%zu)\n", P[z], nL);
                return P[z];
            }

            //printf("(%zu, %zu, %zu)\n", x, y, z);
            L[nL++] = z; // push
            P[z] = -1;

            if(Fo[z] < Fo[y])
            {
                //printf("Fo[z] < Fo[y]\n");
                nLp = 1; // reset
                Lp[0] = z;
                breadth_first = 0;
            } else {
                Lp[nLp++] = z; // push
            }
        }
    }
    /* New label */
    //printf("Asking for a new label (nL=%zu)\n", nL);
    *_nL = nL;
    return 0;
}

int * watershed_cuts(const double * F, size_t M, size_t N)
{
    /* Actual input:
     * (V,E) -- defines a graph
     * F value on edges
     * Here we assume a 4-connected graph over the image
     **/

    // \psi in the paper which will be the output
    // with a label for each pixel
    int * P = calloc(M*N, sizeof(int));
    double * F0 = compute_F0(F, M, N);

    if(M*N < 100)
    {
        printf("F0=\n");
        show_double_matrix(F0, M, N);
    }

    size_t * L = malloc(M*N*sizeof(size_t));
    size_t * Lp = malloc(M*N*sizeof(size_t));

    size_t nb_labs = 0;
    for(size_t kk = 0; kk < M*N; kk++)
    {
        if(P[kk] == 0)
        {
            size_t nL = 0;
            int lab = get_stream(kk,
                                 F, F0,
                                 P,
                                 M, N,
                                 L, Lp, &nL);
#if 0
            printf("nL = %zu, lab=%d\n", nL, lab);
            for(size_t kk=0 ; kk<nL; kk++)
                printf("%zu ", L[kk]);
            printf("\n");
#endif
            if(nL > 0)
            {
                if(lab == 0)
                {
                    /* We create a new label */
                    nb_labs++;
                    lab = nb_labs;
                }

                for(size_t ll = 0; ll < nL; ll++)
                {
                    P[L[ll]] = lab;
                }
                show_int_matrix(P, M, N);
                getchar();
            }
        }
    }

    free(F0);
    free(L);
    free(Lp);
    return P;
}
