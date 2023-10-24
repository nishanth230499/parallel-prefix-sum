#include <math.h>

#include "parallel.h"
using namespace parlay;

/**
 * Inplace block scan DIRECTLY without filter
 */
template <typename T>
T scan(T *A, size_t n) {
  /**
   * REPLACE THE BODY OF THE FOLLOWING
   * FUNCTION WITH YOUR PARALLEL IMPLEMENTATION
  */
  
  // note: 1) Extra functions are allowed 
  //       2) DO NOT change the name of this `T scan(T *A, size_t n)`
  T total = 0;
  for (size_t i = 0; i < n; i++) {
    T tmp = A[i];
    A[i] = total;
    total += tmp;
  }
  return total;
}
