An implementation of [^1].

## TODO
- [ ] Test it
- [ ] Implement also for 3D or the general case.

Note that there are some options:
- 4-connectivity was used here, for 2D images.
- The function on the edges, $`F(\{x,y\})`$, being a function on the edge
  $`\{x,y\}`$ connecting $x$ and $y$ can also be altered, this
  implementation used

``` math
F(\{x,y\}) = \min \left( I(x) , I(y) \right)
```

it was also suggested that
``` math
F(\{x,y\}) = | I(x) - I(y) |
```

could be used.

## Performance
An obvious difference to the standard methods is that there is no
contours around the labeled regions. The actual boundaries are on the
edges, i.e., between the pixels. In the bottom-right image boundary
pixels are artificially introduced (see **matlab/test_watershed_cuts.m**).

<img src="doc/screenshot1.png">

The behavior is different in the case seen below. However in this
particular example it would be a simple thing to mask the output by
the input.

<img src="doc/screenshot2.png">


A quick benchmark on images of size $`[n \times n]`$ reveals that it
is fast, here compared to the watershed implementation in MATLAB
R2020b, and the one in scikit-image (not exactly the same test image
was used for scikit-image):

| n    | watershed [s] | scikit-image | this [s] |
| ---  |    ---        |     ---      |   ---    |
| 256  |  0.014        |   0.018      | 0.0011   |
| 512  |  0.061        |   0.09       | 0.0036   |
| 1024 |  0.26         |   0.73       | 0.016    |
| 2048 |  1.8          |   5.5        | 0.76     |
| 4096 |  9.8          |  32          | 0.33     |
| 8192 | 49            | 170          | 1.3      |

## References

[^1]: Cousty, Jean and Bertrand, Gilles and Najman, Laurent and Couprie, Michel, Watershed Cuts: Minimum Spanning Forests and the Drop of Water Principle, IEEE Transactions on Pattern Analysis and Machine Intelligence, 2009 31(8), pp 1362-1374, [doi:10.1109/TPAMI.2008.173](http://dx.doi.org/10.1109/TPAMI.2008.173)
