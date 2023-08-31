#include "mex.h"

#include "watershed_cuts.h"

void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[])
{
    if(nrhs != 1)
    {
        mexErrMsgTxt("Usage: watershed_cuts(I)\n");
    }

    for(int kk = 0; kk<nrhs; kk++)
    {
        if (!(mxIsDouble(prhs[kk]))) {
            mexErrMsgTxt("Arguments must be of type double.");
        }
    }

    const mwSize ndims = mxGetNumberOfDimensions(prhs[0]);
    const mwSize * dims = mxGetDimensions(prhs[0]);

    if(ndims > 3)
    {
        mexErrMsgTxt("At most 3 dimensions supported\n");
    }

    size_t M = dims[0];
    size_t N = dims[1];
    size_t P = 1;

    if(ndims > 2)
    {
        P = dims[2];
    }

    double * I = mxGetPr(prhs[0]);

    plhs[0] = mxCreateNumericArray(ndims, dims, mxDOUBLE_CLASS, mxREAL);
    double * _D = mxGetPr(plhs[0]);

    int * W = watershed_cuts(I, M, N, P);

    for(size_t kk = 0; kk<M*N*P; kk++)
    {
        _D[kk] = W[kk];
    }


    return;
}
