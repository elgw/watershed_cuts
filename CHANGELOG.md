## v0.0.3
- Changed the return type from int to int64.
- Added an interface to use with pre-allocated output array (good for MATLAB).

## v0.0.2
- Changed the inner loop of **get_stream** from `while` to `for` which
  gained some performance. The code in the previous version reads more
  like the paper.
