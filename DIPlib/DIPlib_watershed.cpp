#include "time.h"

#include "diplib.h"
#include "diplib/morphology.h"
#include "watershed_cuts.h"

double clockdiff(struct timespec* end, struct timespec * start)
{
    double elapsed = (end->tv_sec - start->tv_sec);
    elapsed += (end->tv_nsec - start->tv_nsec) / 1000000000.0;
    return elapsed;
}

static void time_size(size_t M, size_t N, size_t P)
{
    struct timespec tstart, tend;

    double * F = (double *) malloc(M*N*P*sizeof(double));
    for(size_t kk = 0; kk<M*N*P; kk++)
    {
        F[kk] = 0;
    }

    F[0] = -1;

    clock_gettime(CLOCK_REALTIME, &tstart);
    int * W = watershed_cuts(F, M, N, P);
    clock_gettime(CLOCK_REALTIME, &tend);
    double dtWSC = clockdiff(&tend, &tstart);

    free(W);

    dip::Image in;
    in.SetSizes( {M, N, P} );
    in.SetDataType( dip::DT_DFLOAT );
    in.SetTensorSizes( 1 );
    in.Forge();

    dip::Image mask;
    mask.SetSizes( {M, N, P} );
    mask.SetDataType( dip::DT_BIN );
    mask.SetTensorSizes( 1 );
    mask.Forge();

    dip::dfloat * in_pixels = (dip::dfloat*)  in.Origin();
    dip::bin * mask_pixels = (dip::bin*)  mask.Origin();
    for(size_t kk = 0; kk < M*N*P; kk++)
    {
        in_pixels[kk] = (double) rand() / (double) RAND_MAX;
        mask_pixels[kk] = 1;
    }
    free(F);

    dip::Image out; // raw, not forged

    clock_gettime(CLOCK_REALTIME, &tstart);
    dip::Watershed(in, mask, out);
    clock_gettime(CLOCK_REALTIME, &tend);
    double dtDIP = clockdiff(&tend, &tstart);

    printf("[%zu x %zu x %zu] DIP: %f s WSC %f s\n",
           M, N, P,
           dtDIP, dtWSC);
}

int main()
{

    for(size_t N = 16; N <= 512; N*=2)
    {
        time_size(N, N, N);
    }

    return EXIT_SUCCESS;
}
