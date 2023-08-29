An implementation of [^1]. Note that there are some options:
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



An obvious difference to the standard methods is that there is no
contours around the labeled regions. However boundaries can of course
be added if needed. Actually the boundaries are between the pixels so the
depiction below sub optimal.

<img src="doc/screenshot1.png">

Also the behavior is different in the case seen below. Not sure if it
is an intrinsic difference or if I screwed up the implementation.

<img src="doc/screenshot2.png">


A quick benchmark reveals that it is fast, here compared to the
watershed implementation in MATLAB R2020b:

| n | watershed [s]| this [s] |
| --- | --- | --- |
| 256 | 0.014 | 0.0011 |
| 512 | 0.061 | 0.0036 |
| 1024 | 0.26 | 0.016 |
| 2048 | 1.8 | 0.76 |
| 4096 | 9.8 | 0.33 |
| 8192 | 49 | 1.3 |

References

[^1]:  Cousty, Jean and Bertrand, Gilles and Najman, Laurent and Couprie, Michel, Watershed Cuts: Minimum Spanning Forests and the Drop of Water Principle, IEEE Transactions on Pattern Analysis and Machine Intelligence, 2009 31(8), pp 1362-1374, [doi:10.1109/TPAMI.2008.173](http://dx.doi.org/10.1109/TPAMI.2008.173)
