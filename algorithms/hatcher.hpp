/**
 * @file hatcher.hpp
 *
 * @brief Algorithms on the Gregorian calendar by Hatcher [1].
 *
 * This code is a supplementary material to [2].
 *
 *     [1] Hatcher DA., Generalized Equations for Julian Day Numbers and
 *     Calendar Dates. Quarterly Journal of the Royal Astronomical Society
 *     1985; 26(2): p151-155.
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_ALGORITHMS_HATCHER_H
#define EAF_ALGORITHMS_HATCHER_H

#include "eaf/date.hpp"

#include <stdint.h>

struct hatcher {

  // Original epoch: 24 November -4713.
  static constexpr int32_t ajustment = 2440588;

  static inline
  date32_t to_date(int32_t n) {
    int32_t J  = n + ajustment;
    int32_t g  = (3 * ((4 * J - 17918) / 146097) + 2) / 4 - 37;
    int32_t N  = J + g;
    int32_t A  = 4 * N / 1461 - 4712;
    int32_t dp = (4 * N - 237) % 1461 / 4;
    int32_t M  = ((10 * dp + 5) / 306 + 2) % 12 + 1;
    int32_t D  = (10 * dp + 5) % 306 / 10 + 1;

    return { int32_t(A), uint32_t(M), uint32_t(D) };
  }

  static inline
  int32_t to_rata_die(int32_t A, uint32_t M, uint32_t D) {

    int32_t const j  = M < 3;
    int32_t const Ap = j ? A - 1 : A;
    int32_t const Mp = j ? M + 9 : M - 3;
    int32_t const y  = 1461 * (Ap + 4712) / 4;
    int32_t const d  = (306 * int32_t(Mp) + 5) / 10;
    int32_t const N  = y + d + int32_t(D) + 59;
    int32_t const g  = 3 * (Ap / 100 + 49) / 4 - 38;
    int32_t const J  = N - g;

    return J - ajustment;
  }

}; // struct hatcher

#endif // EAF_ALGORITHMS_HATCHER_H
