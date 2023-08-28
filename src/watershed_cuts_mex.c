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

    if(ndims != 2)
    {
        mexErrMsgTxt("Only 2 dimensions supported\n");
    }

    double * I = mxGetPr(prhs[0]);

    plhs[0] = mxCreateNumericArray(ndims, dims, mxDOUBLE_CLASS, mxREAL);
    double * _D = mxGetPr(plhs[0]);

    int * W = watershed_cuts(I, dims[0], dims[1]);

    for(size_t kk = 0; kk<dims[0]*dims[1]; kk++)
    {
        _D[kk] = W[kk];
    }


    return;
}
