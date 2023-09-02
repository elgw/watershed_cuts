An implementation of the surprisingly simple $`O(n)`$ watershed cuts
(WSC) algorithm from Cousty et al. [^1]. While the algorithm is
presented in the framework of undirected graphs the code in this repo
only runs on 2D and 3D images and the graph structure is implicit by
using 4-connectivity in 2D and 6-connectivity in 3D.

There are a few choices to make, one of them is how to define the
function on the edges, $`F(\{x,y\})`$ where $`\{x,y\}`$ denotes the
connecting vertex $x$ and $y$.

This implementation use

``` math
F(\{x,y\}) = \min \left( I(x) , I(y) \right)
```

which give the standard watershed behavior. Another option, suggested
in the paper, is

``` math
F(\{x,y\}) = | I(x) - I(y) |
```

which has the property that it locates the boundaries of the watershed
regions along image edges.

## Performance
An obvious difference to the standard watershed methods is the lack of
contours or watersheds between labeled regions. The actual boundaries
are on the graph edges, i.e., between the pixels.

Below is an example image demonstrating the differences in output from
the **watershed**[^2] function in MATLAB. In the bottom-right
image boundary pixels are artificially introduced (pixels set to 0
when they differ from the erosion by a small structuring element, see
**matlab/test_watershed_cuts.m**).

<img src="doc/screenshot1.png">

Here is another example, borrowed from the documentation of
**watershed** showing the result of watershed cuts to the right.

<img src="doc/screenshot2.png">

A quick benchmark on images of size $`\left[n \times n \times
n\right]`$ reveals that WSC has a competitive
performance.

In the table below this implementation of WSC is compared to the
watershed implementation in MATLAB R2020b. Using two types of images:
"flat", where all pixels are set to 0 and "rand" where pixels got a
random value. Timing are reported in seconds. Interestingly, WSC is
faster on complex images than on simple, this is due to more
cache-misses in the latter case.

| n     | MATLAB/flat | MATLAB/rand | WSC/flat | WSC/rand |
| ---   | ---         | ---         | ---      | ---      |
| 128   | 0.31        | 1.3         | 0.11     | 0.060    |
| 256   | 2.3         | 15          | 1.2      | 0.52     |
| 512   | 19          | 150         | 11       | 5.2      |


## References and alternative implementation

[^1]: Cousty, Jean and Bertrand, Gilles and Najman, Laurent and Couprie, Michel, Watershed Cuts: Minimum Spanning Forests and the Drop of Water Principle, IEEE Transactions on Pattern Analysis and Machine Intelligence, 2009 31(8), pp 1362-1374, [doi:10.1109/TPAMI.2008.173](http://dx.doi.org/10.1109/TPAMI.2008.173) see also [https://perso.esiee.fr/~coustyj/]
[^2]: [MATLAB watershed documentation](https://se.mathworks.com/help/images/ref/watershed.html)
[^3]: [scikit-image documentation on watershed](https://scikit-image.org/docs/stable/api/skimage.segmentation.html#skimage.segmentation.watershed) there is also a [demo](https://scikit-image.org/docs/stable/auto_examples/segmentation/plot_watershed.html)
[^4]: [DIPlib watershed documentation](https://diplib.org/diplib-docs/segmentation.html)
