This benchmark requires and tests the watershed implementation in
[DIPlib](https://github.com/DIPlib/diplib/) to the watershed cuts
implementation in this repo.

The input is cube sized volumetric images with random data, which is
likely to be close to the worst possible input in terms of processing
speed.

Build with cmake:

``` shell
mkdir build
cd build
cmake ../
cmake --build .
```

If DIPlib is properly installed you will not need to

``` shell
export LD_LIBRARY_PATH=/usr/local/lib
```

before you run.

Example output:

``` shell
$ ./DIPlib_watershed
[16 x 16 x 16] DIP: 0.000391 s WSC 0.000268 s
[32 x 32 x 32] DIP: 0.003655 s WSC 0.002145 s
[64 x 64 x 64] DIP: 0.037313 s WSC 0.017821 s
[128 x 128 x 128] DIP: 0.546341 s WSC 0.140678 s
[256 x 256 x 256] DIP: 6.319004 s WSC 1.110842 s
[512 x 512 x 512] DIP: 66.050713 s WSC 8.833543 s
```
