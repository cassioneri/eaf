// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-SnippetCopyrightText: Copyright (C) 1993-2020 Free Software Foundation, Inc.

/**
 * @file libcxx.hpp
 *
 * @brief Algorithms on the Gregorian calendar from libc++ [1].
 *
 * This code is a supplementary material to [2].
 *
 *     [1] https://libcxx.llvm.org
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */
#ifndef EAF_BENCHMARK_LIBCXX_HPP
#define EAF_BENCHMARK_LIBCXX_HPP

#include "date.hpp"

#include <cstdint>

struct libcxx {

  // Original epoch: 1 January 1970.

  // https://tinyurl.com/yd7odvjd
  static inline
  date32_t to_date(int32_t __d) noexcept {

    const int      __z = __d + 719468;
    const int      __era = (__z >= 0 ? __z : __z - 146096) / 146097;
    const unsigned __doe = static_cast<unsigned>(__z - __era * 146097);              // [0, 146096]
    const unsigned __yoe = (__doe - __doe/1460 + __doe/36524 - __doe/146096) / 365;  // [0, 399]
    const int      __yr = static_cast<int>(__yoe) + __era * 400;
    const unsigned __doy = __doe - (365 * __yoe + __yoe/4 - __yoe/100);              // [0, 365]
    const unsigned __mp = (5 * __doy + 2)/153;                                       // [0, 11]
    const unsigned __dy = __doy - (153 * __mp + 2)/5 + 1;                            // [1, 31]
    const unsigned __mth = __mp + (__mp < 10 ? 3 : -9);                              // [1, 12]

    return { int32_t(__yr + (__mth <= 2)), uint32_t(__mth), uint32_t(__dy) };
  }

  // https://tinyurl.com/y8ja66lz
  static inline
  int32_t to_rata_die(int32_t year, uint32_t month, uint32_t day) {

    const int      __yr  = static_cast<int>(year) - (month <= 2);
    const unsigned __mth = static_cast<unsigned>(month);
    const unsigned __dy  = static_cast<unsigned>(day);
    const int      __era = (__yr >= 0 ? __yr : __yr - 399) / 400;
    const unsigned __yoe = static_cast<unsigned>(__yr - __era * 400);                // [0, 399]
    const unsigned __doy = (153 * (__mth + (__mth > 2 ? -3 : 9)) + 2) / 5 + __dy-1;  // [0, 365]
    const unsigned __doe = __yoe * 365 + __yoe/4 - __yoe/100 + __doy;                // [0, 146096]

    return int32_t(__era * 146097 + static_cast<int>(__doe) - 719468);
  }

}; // struct libcxx

#endif // EAF_BENCHMARK_LIBCXX_HPP
