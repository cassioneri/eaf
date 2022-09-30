/**
 * @file fliegel_flandern.hpp
 *
 * @brief Algorithms on the Gregorian calendar by Fliegel and Flandern [1].
 *
 * This code is a supplementary material to [2].
 *
 *     [1] H.F. Fliegel and T.C.V. Flandern, A Machine Algorithm for
 *     Processing Calendar Dates. Communications of the ACM, Vol. 11, No. 10
 *     (1968), p657.
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_BENCHMARK_FLIEGEL_FLANDERN_HPP
#define EAF_BENCHMARK_FLIEGEL_FLANDERN_HPP

#include "date.hpp"

#include <cstdint>

struct fliegel_flandern {

  // Original epoch: 24 November -4713.
  static constexpr int32_t ajustment = 2440588;

  static inline
  date32_t to_date(int32_t JD) {

    int32_t       L  = JD + 68569 + ajustment;
    int32_t const N  = 4 * L / 146097;
                  L  = L - (146097 * N + 3) / 4;
    int32_t       I  = 4000 * (L + 1) / 1461001;
                  L  = L - 1461 * I / 4 + 31;
    int32_t       J  = 80 * L / 2447;
    int32_t const K  = L - 2447 * J / 80;
                  L  = J / 11;
                  J  = J + 2 - 12 * L;
                  I  = 100 * (N - 49) + I + L;

    return { int32_t(I), uint32_t(J), uint32_t(K) };
  }

  static inline
  int32_t to_rata_die(int32_t y, uint32_t m, uint32_t d) {
    int32_t const I = y;
    int32_t const J = m;
    int32_t const K = d;
    int32_t const JD = K - 32075 + 1461 * (I + 4800 + (J - 14) / 12) / 4
      + 367 * (J - 2 - (J - 14) / 12 * 12) / 12 - 3
      * ((I + 4900 + (J - 14) / 12) / 100) / 4;
    return JD - ajustment;
  }

}; // struct fliegel_flandern

#endif // EAF_BENCHMARK_FLIEGEL_FLANDERN_HPP
