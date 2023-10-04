#include "watershed_cuts.h"
#include <stdio.h>


/*
 * Alternatives discussed in the paper are:
 * F(edge) = F({x,y}) = MIN(I(x), I(y)) which is used here
 * and F({x,y}) = |I(x)-I(y)|
 * No need to pre-compute F0, but that might be faster?
 * Cache-misses must be the main performance problem
 */


static double MIN(const double x, const double y)
{
    if( x < y)
    {
        return x;
    }
    return y;
}

/* Note:
 * this is based on F({x,y}) = min(I(x), I(y))
 * where {x,y} is the edge connecting the vertices/pixels
 * x, y
 */
static double * compute_F0_min(const double * restrict F,
                               const uint64_t M, const uint64_t N, const uint64_t P)
{
    /* Loop over the first dimension */
    double * F0 = malloc(M*N*P*sizeof(double));
    for(uint64_t pp = 0; pp < P; pp++)
    {
        for(uint64_t ll = 0; ll < N; ll++)
        {
            F0[M*ll + M*N*pp] = MIN(F[ll*M + M*N*pp], F[1 + ll*M + M*N*pp]);
            for(uint64_t kk = 1; kk+1 < M; kk++)
            {
                F0[kk+M*ll + M*N*pp] = MIN(F[kk+1 + ll*M + M*N*pp],
                                           MIN(F[kk-1 + ll*M + M*N*pp],
                                               F[kk + ll*M + M*N*pp]));
            }
            F0[M-1 + M*ll + M*N*pp] = MIN(F[M-2 + ll*M + M*N*pp],
                                          F[M-1 + ll*M + M*N*pp]);
        }
    }
    /* Loop over the 2nd dimensions */

    for(uint64_t pp = 0; pp < P; pp++)
    {
        for(uint64_t kk = 0; kk < M; kk++)
        {
            uint64_t ll = 0;
            F0[kk+M*ll + M*N*pp] = MIN(F0[kk+ll*M + M*N*pp],
                                       F[kk+(ll+1)*M + M*N*pp]);
        }

        for(uint64_t kk = 0; kk < M; kk++)
        {
            uint64_t ll = N-1;
            F0[kk+M*ll + M*N*pp] = MIN(F0[kk+ll*M + M*N*pp],
                                       F[kk+(ll-1)*M + M*N*pp]);
        }

        for(uint64_t ll = 1; ll+1 < N; ll++)
        {
            for(uint64_t kk = 0; kk < M; kk++)
            {
                F0[kk+M*ll + M*N*pp] = MIN(F[kk+(ll+1)*M + M*N*pp],
                                           MIN(F[kk + (ll-1)*M + M*N*pp],
                                               F0[kk+ll*M + M*N*pp]));
            }
        }
    }

    if(P > 1)
    {
        for(uint64_t pp = 0; pp<1; pp++)
        {
            for(uint64_t ll = 0; ll<N; ll++)
            {
                for(uint64_t kk = 0; kk<M; kk++)
                {
                    F0[kk+M*ll + M*N*pp] = MIN( F[kk+ll*M + M*N*(pp+1)],
                                                F0[kk+ll*M + M*N*pp] );
                }
            }
        }

        for(uint64_t pp = 1; pp+1<P; pp++)
        {
            for(uint64_t ll = 0; ll<N; ll++)
            {
                for(uint64_t kk = 0; kk<M; kk++)
                {
                    F0[kk+M*ll + M*N*pp] = MIN(F[kk+ll*M + M*N*(pp-1)],
                                               MIN(F[kk+ll*M + M*N*(pp+1)],
                                                   F0[kk+ll*M + M*N*pp]));
                }
            }
        }

        for(uint64_t pp = P-1; pp<P; pp++)
        {
            for(uint64_t ll = 0; ll<N; ll++)
            {
                for(uint64_t kk = 0; kk<M; kk++)
                {
                    F0[kk+M*ll + M*N*pp] = MIN( F[kk+ll*M + M*N*(pp-1)],
                                                F0[kk+ll*M + M*N*pp] );
                }
            }
        }
    }

    return F0;
}


static int64_t
find_neighbour_ids(uint64_t * restrict Z,
                   const uint64_t y,
                   const uint64_t M, const uint64_t N, const uint64_t P)
{
    uint64_t nZ = 0;

    uint64_t m, n, p = 0;

    ldiv_t d = ldiv(y, M*N);
    p = d.quot;
    d = ldiv(y-p*M*N, M);
    n = d.quot;
    m = d.rem;

    if(m+1 < M)
    {
        Z[nZ++] = y+1;
    }

    if(n+1 < N)
    {
        Z[nZ++] = y + M;
    }

    if(m > 0)
    {
        Z[nZ++] = y - 1;
    }

    if(n > 0)
    {
        Z[nZ++] = y - M;
    }

    if(P > 1)
    {
        if(p+1 < P)
        {
            Z[nZ++] = y + M*N;
        }
        if(p > 0)
        {
            Z[nZ++] = y - M*N;
        }
    }

    return nZ;
}


static int64_t get_stream(const uint64_t x,
                      const double * restrict F,
                      const double * restrict Fo,
                      int64_t * restrict PSI,
                      const uint64_t M, const uint64_t N, const uint64_t P,
                      uint64_t * restrict L, uint64_t * restrict Lp, uint64_t * _nL)
{
    // x is the start pixel
    // Lp is s stack for pixels to visit
    // L is a stack for the pixels in the stream

    uint64_t nL = 0; // Empty stream
    uint64_t nLp = 0;

    L[nL++] = x;
    PSI[x] = -1; // temporary label
    Lp[nLp++] = x;
    uint64_t Z[6];

    while(nLp > 0)
    {
        uint64_t y = Lp[--nLp]; // pop

        uint64_t nZ = find_neighbour_ids(Z, y, M, N, P);
        for(uint64_t kk = 0; kk < nZ; kk++)
        {
            uint64_t z = Z[kk];

            if( PSI[z] == -1 )
            {
                continue;
            }

            if( MIN(F[z], F[y]) != Fo[y] )
            {
                continue;
            }

            if(PSI[z] > 0) // if already has a label
            {
                /* Merge with existing label */
                *_nL = nL;
                //printf("Found label %d (nL=%zu)\n", P[z], nL);
                return PSI[z];
            }

            //printf("(%zu, %zu, %zu)\n", x, y, z);
            L[nL++] = z; // push
            PSI[z] = -1;

            if(Fo[z] < Fo[y])
            {
                //printf("Fo[z] < Fo[y]\n");
                nLp = 1; // reset
                Lp[0] = z;
                break;
            } else {
                Lp[nLp++] = z; // push
            }
        }

    }
    /* New label */
    //printf("Asking for a new label (nL=%zu)\n", nL);
    *_nL = nL;
    return -1;
}

int watershed_cuts_pre(int64_t * restrict PSI, const double * F,
                     uint64_t M, uint64_t N, uint64_t P)
{
    /* Actual input:
     * (V,E) -- defines a graph
     * F value on edges
     * Here we assume a 4-connected graph over the image
     **/

    if(F == NULL)
    {
        return EXIT_FAILURE;
    }
    if(PSI == NULL)
    {
        return EXIT_FAILURE;
    }

    if(M*N == 0)
    {
        return EXIT_FAILURE;
    }

    // PSI is \psi in the paper which will be the output
    // with a label for each pixel

    /* F^o in the paper. MIN alternative
     * This does not need to be pre-computed. I'm betting it is
     * faster to do that.
     */
    double * F0 = compute_F0_min(F, M, N, P);

    /* Temporary stacks for the stream calculations. Allocate to the
     *  worst case scenario, i.e. all pixels in the stacks.
     */

    uint64_t * L = malloc(M*N*P*sizeof(uint64_t));
    uint64_t * Lp = malloc(M*N*P*sizeof(uint64_t));

    uint64_t nb_labs = 0; /* Number of labels used */
    for(uint64_t kk = 0; kk < M*N*P; kk++)
    {
        if(PSI[kk] == 0)
        {
            uint64_t nL = 0;
            int64_t lab = get_stream(kk,
                                 F, F0,
                                 PSI,
                                 M, N, P,
                                 L, Lp, &nL);

            if(nL > 0)
            {
                if(lab == -1)
                {
                    /* We create a new label */
                    nb_labs++;
                    lab = nb_labs;
                    // printf("Lab=%d\n", lab);
                }

                for(uint64_t ll = 0; ll < nL; ll++)
                {
                    PSI[L[ll]] = lab;
                }
            }
        }
    }

    free(F0);
    free(L);
    free(Lp);
    return EXIT_SUCCESS;
}

int64_t *
watershed_cuts(const double * I,
               uint64_t M, uint64_t N, uint64_t P)
{
    if(M*N*P == 0)
    {
        return NULL;
    }
    int64_t * W = calloc(M*N*P, sizeof(int64_t));
    if(W == NULL)
    {
        return NULL;
    }
    if(watershed_cuts_pre(W, I, M, N, P))
    {
        free(W);
        return NULL;
    }
    return W;
}
