#include <math.h>

#include "parallel.h"
using namespace parlay;

/**
 * Inplace block scan DIRECTLY without filter
 */
template <typename T>
T scan(T *A, size_t n) {
  size_t k = 10000;
  size_t chunk_size = (size_t) n/k + 1;
  T *chunk_sum = (T *)malloc(k * sizeof(T));

  parallel_for(0, k, [&](size_t i) {
    chunk_sum[i] = 0;
    for(size_t j = 0; j < chunk_size && (i * chunk_size + j) < n; j++) {
      chunk_sum[i] += A[i * chunk_size + j];
    }
  });

  A[chunk_size - 1] = chunk_sum[0];
  for(size_t i = 1; i < k; i++) {
    if(i + 1 == k) {
      A[n - 1] = A[(i) * chunk_size - 1] + chunk_sum[i];
    }
    else {
      A[(i+1) * chunk_size - 1] = A[(i) * chunk_size - 1] + chunk_sum[i];
    }
  }

  free(chunk_sum);

  for (size_t i = 1; i < chunk_size - 1; i++) {
    A[i] += A[i-1];
  }

  parallel_for(1, k, [&](size_t i) {
    for(size_t j = 0; j < chunk_size - 1 && (i * chunk_size + j) < n - 1; j++) {
      A[i * chunk_size + j] += A[i * chunk_size + j - 1];
    }
  });

  return A[n - 1];
}
