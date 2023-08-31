An implementation of the $`O(n)`$ watershed algorithm from Cousty et al. [^1].

The paper presents some options for the implementer.
- This code uses 4-connectivity, and is only 2D and 3D images. Since
  the me
- The function on the edges, $`F(\{x,y\})`$, being a function on the edge
  $`\{x,y\}`$ connecting $x$ and $y$ can also be altered, this
  implementation uses

``` math
F(\{x,y\}) = \min \left( I(x) , I(y) \right)
```

- Another option that was suggested, to stop the regions at the edges was:

``` math
F(\{x,y\}) = | I(x) - I(y) |
```

## Performance
An obvious difference to the standard methods is that there are no
contours around the labeled regions. The actual boundaries are on the
edges, i.e., between the pixels. In the bottom-right image boundary
pixels are artificially introduced (see **matlab/test_watershed_cuts.m**).

<img src="doc/screenshot1.png">

Here is another example:
<img src="doc/screenshot2.png">


A quick benchmark on images of size $`\left[n \times n\right]`$ reveals that it
is fast, here compared to the watershed implementation in MATLAB
R2020b, and the one in scikit-image (not exactly the same test image
was used for scikit-image):

| n    | watershed [s] | scikit-image [s] | this [s] |
| ---  |    ---        |     ---          |   ---    |
| 256  |  0.014        |   0.018          | 0.0011   |
| 512  |  0.061        |   0.09           | 0.0036   |
| 1024 |  0.26         |   0.73           | 0.016    |
| 2048 |  1.8          |   5.5            | 0.76     |
| 4096 |  9.8          |  32              | 0.33     |
| 8192 | 49            | 170              | 1.3      |


## TODO
- [ ] Test it a little more
- [ ] Explore the options


## References

[^1]: Cousty, Jean and Bertrand, Gilles and Najman, Laurent and Couprie, Michel, Watershed Cuts: Minimum Spanning Forests and the Drop of Water Principle, IEEE Transactions on Pattern Analysis and Machine Intelligence, 2009 31(8), pp 1362-1374, [doi:10.1109/TPAMI.2008.173](http://dx.doi.org/10.1109/TPAMI.2008.173)
