// SPDX-License-Identifier: MIT
// SPDX-SnippetCopyrightText: Copyright (C) 1993-2020 Free Software Foundation, Inc.

/**
 * @file glibc.hpp
 *
 * @brief Algorithms on the Gregorian calendar from glibc [1].
 *
 * This code is a supplementary material to [2].
 *
 *     [1] https://www.gnu.org/software/libc
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_ALGORITHMS_GLIBC_HPP
#define EAF_ALGORITHMS_GLIBC_HPP

#include "eaf/date.hpp"

#include <cstdint>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

struct glibc {

  // To avoid undefined behaviour, we have replaced names __is_leap and
  // __mon_yday, resp., with underscore_isleap and undesrcore_mon_yday.

  // https://tinyurl.com/y7y3doog
  #define underscore_isleap(year) \
    ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

  // https://tinyurl.com/ycdpf72e
  static inline
  date32_t to_date(int32_t days) {

    int64_t y = 1970;
    #define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
    #define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

    while (days < 0 || days >= (underscore_isleap (y) ? 366 : 365))
      {
        /* Guess a corrected year, assuming 365 days per year.  */
        long int yg = y + days / 365 - (days % 365 < 0);

        /* Adjust DAYS and Y to match the guessed year.  */
        days -= ((yg - y) * 365
                + LEAPS_THRU_END_OF (yg - 1)
                - LEAPS_THRU_END_OF (y - 1));
        y = yg;
      }

    const unsigned short int *ip = undesrcore_mon_yday[underscore_isleap(y)];
    long int m = 0;
    for (m = 11; days < (long int) ip[m]; --m)
      continue;
    days -= ip[m];
    return { int32_t(y), uint32_t(m + 1), uint32_t(days + 1) };
  }

  // https://tinyurl.com/ycz86upl
  #define EPOCH_YEAR 1970
  #define TM_YEAR_BASE 1900

  // https://tinyurl.com/yap87jlu
  static inline
  int32_t to_rata_die(int32_t y, uint32_t month , uint32_t day) {
    int mday = day;
    int mon = month - 1;
    long int year = y - TM_YEAR_BASE;
    int mon_yday = (undesrcore_mon_yday[leapyear(year)][mon]) - 1;
    long int yday = mon_yday + mday;
    long int t0 = ydhms_diff(year, yday, EPOCH_YEAR - TM_YEAR_BASE);
    return t0;
  }

private:

  // https://tinyurl.com/y9bkozw6
  static unsigned short int constexpr undesrcore_mon_yday[2][13] =
    {
      /* Normal years.  */
      { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
      /* Leap years.  */
      { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
    };

  // https://tinyurl.com/ydhzwd4n
  static inline
  long int shr(long int a, int b) {
    long int one = 1;
    return (-one >> 1 == -1 ? a >> b : a / (one << b) - (a % (one << b) < 0));
  }

  // https://tinyurl.com/ya33text
  static inline
  bool leapyear(long int year) {
    return ((year & 3) == 0 && (year % 100 != 0 ||
      ((year / 100) & 3) == (- (TM_YEAR_BASE / 100) & 3)));
  }

  // https://tinyurl.com/y7qrapyo
  static inline
  long int ydhms_diff (long int year1, long int yday1, int year0) {
    int a4 = shr (year1, 2) + shr (TM_YEAR_BASE, 2) - ! (year1 & 3);
    int b4 = shr (year0, 2) + shr (TM_YEAR_BASE, 2) - ! (year0 & 3);
    int a100 = a4 / 25 - (a4 % 25 < 0);
    int b100 = b4 / 25 - (b4 % 25 < 0);
    int a400 = shr (a100, 2);
    int b400 = shr (b100, 2);
    int intervening_leap_days = (a4 - b4) - (a100 - b100) + (a400 - b400);
    long int years = year1 - year0;
    long int days = 365 * years + yday1 + intervening_leap_days;
    return days;
  }

}; // struct glibc

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// Clean-up:
#undef TM_YEAR_BASE
#undef EPOCH_YEAR
#undef LEAPS_THRU_END_OF
#undef DIV
#undef underscore_isleap

#endif // EAF_ALGORITHMS_GLIBC_HPP
