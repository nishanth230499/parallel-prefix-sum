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
  size_t k = 10000;
  size_t chunk_size = (size_t) n/k + 1;
  T *chunk_sum = (Type *)malloc(k * sizeof(T));

  parallel_for(0, k, [&](size_t i) {
    chunk_sum[k] = 0;
    for(size_t j = 0; j < chunk_size && (k * chunk_size + j) < n; j++) {
      chunk_sum[k] += A[k * chunk_size + j];
    }
  });

  A[chunk_size - 1] = S[0];
  for(size_t i = 1; i < k; i++) {
    if(i + 1 == k) {
      A[n - 1] = A[(i) * chunk_size - 1] + chunk_sum[i];
    }
    else {
      A[(i+1) * chunk_size - 1] = A[(i) * chunk_size - 1] + chunk_sum[i];
    }
  }

  free(chunk_sum);

  return A[n - 1];

  // for (size_t i = 0; i < n; i++) {
  //   T tmp = A[i];
  //   A[i] = total;
  //   total += tmp;
  // }
  // return total;
}
