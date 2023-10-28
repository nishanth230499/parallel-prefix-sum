#include <math.h>

#include "parallel.h"
using namespace parlay;

/**
 * Inplace block scan DIRECTLY without filter
 */
template <typename T>
T scan(T *A, size_t n) {
  size_t k = sqrt(n);
  size_t chunk_size = (size_t) n/k + 1;
  T *chunk_sum = (T *)malloc(k * sizeof(T));
  T total = 0;
  T temp, end_of_chunk;

  parallel_for(0, k, [&](size_t i) {
    chunk_sum[i] = 0;
    for(size_t j = 0; j < chunk_size - 1 && (i * chunk_size + j) < n - 1; j++) {
      chunk_sum[i] += A[i * chunk_size + j];
    }
  });

  for(size_t i = 0; i < k; i++) {
    if(i + 1 == k) {
      end_of_chunk = n - 1;
    } else {
      end_of_chunk = (i + 1) * chunk_size - 1;
    }
    total += chunk_sum[i];
    temp = A[end_of_chunk];
    A[end_of_chunk] = total;
    total += temp;
  }

  free(chunk_sum);

  parallel_for(0, k, [&](size_t i) {
    if(i + 1 == k) {
      end_of_chunk = n - 1;
    } else {
      end_of_chunk = (i + 1) * chunk_size - 1;
    }
    for(size_t j = end_of_chunk; j > i * chunk_size; j--) {
      A[j - 1] = A[j] - A[j - 1];
    }
  });

  return total;
}
