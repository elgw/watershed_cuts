#ifndef watershed_cuts_h_
#define watershed_cuts_h_

#include <stdlib.h>
#include <string.h>

/* An implementation of
 *
 * Cousty, Jean and Bertrand, Gilles and Najman, Laurent and Couprie, Michel,
 * Watershed Cuts: Minimum Spanning Forests and the Drop of Water Principle,
 * IEEE Transactions on Pattern Analysis and Machine Intelligence,
 * 2009 31(8), pp 1362-1374,
 * http://dx.doi.org/10.1109/TPAMI.2008.173
 *
 * Which is a O(n) method, where n is the number of pixels in the
 * image. Previous/typical watershed methods/implementation are O(n*log(n)).
 *
 * Please note that this method does not produce any boundary lines between the
 * labeled regions.
 *
 * This implementation is for 2D images only (while the method is formulated for
 * general undirected graphs). 4-connectivity is used to define the graph over
 * the image, and hence edge pixels only have 3 neighbors and corner pixels
 * only 2.
 *
 * The "Function on the edges" F({x,y}) = MIN(I(x), I(y)) which should
 * provide results most similar to classical water shed
 * implementation. Other options are possible.
 *
 * Input:
 *   A "height matrix" of size [M x N x P] where M is the size of
 *   the non-strided dimension.
 *
 * Output:
 *    An array of the same size as I where the pixel values are interpreted as:
 *    1=first object, 2=second objects, ...
 *    Returns NULL on failure.
 *
 * Limitations:
 *    Can obviously only label up to INT_MAX regions.
 *    The method only uses one thread, since, besides pre-calculating F0, it
 *    does not invite parallelizations.
 *
 * Erik Wernersson, 2023
 */

#define WATERSHED_CUTS_VERSION_MAJOR "0"
#define WATERSHED_CUTS_VERSION_MINOR "0"
#define WATERSHED_CUTS_VERSION_PATCH "1"
#define watershed_cuts_version WATERSHED_CUTS_VERSION_MAJOR "." \
    WATERSHED_CUTS_VERSION_MINOR "."                            \
    WATERSHED_CUTS_VERSION_PATCH

int *
watershed_cuts(const double * I,
               size_t M, size_t N, size_t P);

#endif
