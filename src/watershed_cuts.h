#ifndef watershed_cuts_h_
#define watershed_cuts_h_

#include <stdlib.h>
#include <string.h>

/* Output:
 * 0=no label */
int *
watershed_cuts(const double * I, size_t M, size_t N);

#endif
