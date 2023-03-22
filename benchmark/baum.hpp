/**
 * @file baum.hpp
 *
 * @brief Algorithms on the Gregorian calendar by Baum [1].
 *
 * This code is a supplement material to [2].
 *
 *     [1] https://www.researchgate.net/profile/Peter-Baum
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_BENCHMARK_BAUM_HPP
#define EAF_BENCHMARK_BAUM_HPP

#include "date.hpp"

#include <cstdint>

struct baum {

  // https://tinyurl.com/y44rgx2j

  // Original epoch: 31 December 0000.
  static constexpr int32_t ajustment = 719163;

  // Section 6.2.1/3
  static inline
  date32_t to_date(int32_t n) {

    int32_t const z  = n + 306 + ajustment;
    int32_t const h  = 100 * z - 25;
    int32_t const a  = h / 3652425;
    int32_t const b  = a - a / 4;
    int32_t const y_ = (100 * b + h) / 36525;
    int32_t const c  = b + z - 365 * y_ - y_ / 4;
    int32_t const m_ = (535 * c + 48950) / 16384;
    int32_t const d  = c - (979 * m_ - 2918) / 32;
    int32_t const j  = m_ > 12;
    int32_t const y  = y_ + j;
    int32_t const m  = j ? m_ - 12 : m_;

    return { int32_t(y), uint32_t(m), uint32_t(d) };
  }

  // Section 5.1
  static inline
  int32_t to_rata_die(int32_t year, uint32_t month, uint32_t day) {

    int32_t const j = month < 3;
    int32_t const z = year - j;                                            // step 1 / alternative 2
    int32_t const m = j ? month + 12 : month;                              // step 2 / alternative 3
    int32_t const f = (979 * m - 2918) / 32;                               //
    int32_t const n = day + f + 365 * z + z / 4 - z / 100 + z / 400 - 306; // step 3

    return int32_t(n) - ajustment;
  }

}; // struct baum

#endif // EAF_BENCHMARK_BAUM_HPP
