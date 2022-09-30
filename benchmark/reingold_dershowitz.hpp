/**
 * @file reingold_dershowitz.hpp
 *
 * @brief Algorithms on the Gregorian calendar by Reingold and Dershowitz.
 *
 * This code is a supplementary material to [2].
 *
 *     [1] E.M. Reingold and N.Dershowitz, Calendrical Calculations, The
 *     Ultimate Edition, Cambridge University Press, 2018.
 *
 *     [2] Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_BENCHMARK_REINGOLD_DERSHOWITZ_HPP
#define EAF_BENCHMARK_REINGOLD_DERSHOWITZ_HPP

#include "date.hpp"

#include <cstdint>

struct reingold_dershowitz {

  // Original epoch: 31 December 0000.
  static constexpr int32_t ajustment = 719163;

  // alt-fixed-from-gregorian, equation (2.28), page 65.
  static inline
  int32_t to_rata_die(int32_t y, uint32_t m, uint32_t d) {

    int32_t const year  = int32_t(y);
    int32_t const month = int32_t(m);
    int32_t const day   = int32_t(d);

    // mp := (month - 3) mod 12. If month - 3 < 0, then % doesn't match mod.
    // The below provides the intended result and keeps performance of the
    // original formula.
    int32_t const mp = (month + 9) % 12;
    int32_t const yp = year - mp / 10;

    // On page 66, quantities below are denoted by n400, n100, n4 and n1.
    // Equation (1.42), page 28, with b = <4, 25, 4>, i.e., b0 = 4, b1 = 25
    // and b2 = 4 gives
    int32_t const a0 = (yp / 400);
    int32_t const a1 = (yp / 100) %  4;
    int32_t const a2 = (yp /   4) % 25;
    int32_t const a3 = (yp /   1) %  4;

    int32_t const n = gregorian_epoch - 1 - 306 + 365 * yp + 97 * a0 + 24 * a1 + 1 * a2 + 0 * a3 +
      (3 * mp + 2) / 5 + 30 * mp + day;

    return n - ajustment;
  }

  // alt-gregorian-from-fixed, equation (2.29), page 66.
  static inline
  date32_t to_date(int32_t date) {

    date = date + ajustment;
    int32_t const y          = gregorian_year_from_fixed(gregorian_epoch - 1 + date + 306);
    int32_t const prior_days = date - fixed_from_gregorian(y - 1, 3, 1);
    int32_t const month      = mod_1_12((5 * prior_days + 2) / 153 + 3);
    int32_t const year       = y - (month + 9) / 12;
    int32_t const day        = date - fixed_from_gregorian(year, uint32_t(month), 1) + 1;

    return { int32_t(year), uint32_t(month), uint32_t(day) };
  }

private:

  // Table 1.2, page 17.
  static int32_t constexpr gregorian_epoch = 1;

  // gregorian-year-from-fixed, equation (2.21), page 61.
  static inline
  int32_t gregorian_year_from_fixed(int32_t const& date) {
    int32_t const d0   = date - gregorian_epoch;
    int32_t const n400 = d0 / 146097;
    int32_t const d1   = d0 % 146097;
    int32_t const n100 = d1 / 36524;
    int32_t const d2   = d1 % 36524;
    int32_t const n4   = d2 / 1461;
    int32_t const d3   = d2 % 1461;
    int32_t const n1   = d3 / 365;
    int32_t const year = 400 * n400 + 100 * n100 + 4 * n4 + n1;
    return (n100 == 4 || n1 == 4) ? year : year + 1;
  }

  // alt-fixed-from-gregorian, equation (2.28), page 65.
  static inline
  int32_t fixed_from_gregorian(int32_t y, uint32_t m, uint32_t d) {
    return to_rata_die(y, m, d) + 719163;
  }

  static inline
  int32_t mod_1_12(int32_t month) {
    return month > 12 ? month - 12 : month;
  }

}; // struct reingold_dershowitz

#endif // EAF_BENCHMARK_REINGOLD_DERSHOWITZ_HPP
