// SPDX-License-Identifier: MIT
// SPDX-SnippetCopyrightText: 2002-2003 .NET Foundation and Contributors

/**
 * @file dotnet.hpp
 *
 * @brief Algorithms on the Gregorian calendar from .NET [1].
 *
 * This code is a supplementary material to [2].
 *
 *     [1] https://dotnet.microsoft.com
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_BENCHMARK_DOTNET_HPP
#define EAF_BENCHMARK_DOTNET_HPP

#include "date.hpp"

#include <cstdint>

struct dotnet {

  // Original epoch: 1 January 1.
  static constexpr int32_t ajustment = 719162;

  // https://tinyurl.com/ybyoqjep
  static inline
  date32_t to_date(int32_t rata_die) {

    int n = rata_die + ajustment;
    int y400 = n / DaysPer400Years;
    n -= y400 * DaysPer400Years;
    int y100 = n / DaysPer100Years;
    if (y100 == 4) y100 = 3;
    n -= y100 * DaysPer100Years;
    int y4 = n / DaysPer4Years;
    n -= y4 * DaysPer4Years;
    int y1 = n / DaysPerYear;
    if (y1 == 4) y1 = 3;
    int year = y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
    n -= y1 * DaysPerYear;
    bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
    int const* days = leapYear ? s_daysToMonth366 : s_daysToMonth365;
    int m = (n >> 5) + 1;
    while (n >= days[m]) m++;
    int month = m;
    int day = n - days[m - 1] + 1;

    return { int32_t(year), uint32_t(month), uint32_t(day) };
  }

  // https://tinyurl.com/y7r8u6zo
  static inline
  int32_t to_rata_die(int32_t year, uint32_t month, uint32_t day) {

    int const* days = IsLeapYear(year) ? s_daysToMonth366 : s_daysToMonth365;
    int y = year - 1;
    int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;

    return n - ajustment;
  }

private:

  // https://tinyurl.com/y85q3qrp
  static int constexpr s_daysToMonth365[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
  static int constexpr s_daysToMonth366[] = {
    0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

  // https://tinyurl.com/yctslxyt
  static int constexpr DaysPerYear = 365;
  static int constexpr DaysPer4Years = DaysPerYear * 4 + 1;       // 1461
  static int constexpr DaysPer100Years = DaysPer4Years * 25 - 1;  // 36524
  static int constexpr DaysPer400Years = DaysPer100Years * 4 + 1; // 146097

  // https://tinyurl.com/ycegsjyu
  static inline
  bool IsLeapYear(int32_t year) {
    return (year & 3) == 0 && ((year & 15) == 0 || (year % 25) != 0);
  }

}; // struct dotnet

#endif // EAF_BENCHMARK_DOTNET_HPP
