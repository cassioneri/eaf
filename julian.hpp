// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file julian.hpp
 *
 * @brief Algorithms on the Julian calendar.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_JULIAN_HPP
#define EAF_JULIAN_HPP

#include "common.hpp"

#include <cstdint>

namespace eaf {
namespace julian {

/**
 * @brief Limits for the algorithms.
 */
template <typename T>
struct limits;

template <>
struct limits<int32_t> {

  // N_min    = -2^29 + 365         = -536,870,547,
  // Date     = 18 September -1,469,872.
  static int64_t constexpr N_min    = -536870547;

  // N_max    = 2^29 - 1            =  536,870,911,
  // Date     = 11 August     1,469,872.
  static int64_t constexpr N_max    =  536870911;

  // year_min = (-2^31 + 3) / 1461  = -1,469,872,
  // N        = -536,870,748.
  static int64_t constexpr year_min = -1469872;

  // year_max = 2^31 / 1461 + 1     =  1,469,873,
  // N        =  536,871,112.
  static int64_t constexpr year_max =  1469873;
};

template <>
struct limits<int64_t> {

  // N_min    = -2^61 + 365         = -2,305,843,009,213,693,587,
  // Date     = 25 January  -6,313,054,097,778,764.
  static int64_t constexpr N_min    = -2305843009213693587;

  // N_max    = 2^61 - 1            = 2,305,843,009,213,693,951,
  // Date     =  5 April     6,313,054,097,778,765.
  static int64_t constexpr N_max    =  2305843009213693951;

  // year_min = (-2^63 + 3) / 1461  = -6,313,054,097,778,765,
  // N        = -2,305,843,009,213,693,917.
  static int64_t constexpr year_min = -6313054097778765;

  // year_max = 2^63 / 1461 + 1     =  6,313,054,097,778,766,
  // N        =  2,305,843,009,213,694,280.
  static int64_t constexpr year_max =  6313054097778766;
};

/**
 * @brief Finds the proleptic Julian date from its rata die.
 *
 * The epoch is 1 March 0000 of the proleptic Julian calendar.
 *
 * @tparam T        Year and rata die type.
 *
 * @param N         The rata die.
 *
 * @return The proleptic Julian date.
 */
template <typename T>
date_t<T> constexpr
to_date(T N) noexcept {

  // Let S in {32, 64} be the size of T in bits. If N >=0, then the
  // evaluation of N_1 overflows if, and only if, 4 * N + 3 >= 2^(S - 1) --
  // one more than the maximum value of T. This happens if, and only if,
  // N >= 2^(S - 3). Reciprocally, if 0 <= N < 2^(S - 3), then the
  // calculation of N_1 does not overflow. N_1 is used in the evaluations of
  // quotient(N_1,   1461), which performs N_1 /   1461, yielding a correct
  // and smaller value than N_1, and of remainder(N_1,   1461), which simply
  // calculates N_1 %   1461, yielding a correct result in [0,   1461[.
  // Therefore, if 0 <= N < 2^(S - 3), then the calculations of N_1, Y and
  // N_Y are safe. The code caries on using N_Y which is small enough for
  // all other calculations to be comfortably inside the range of their
  // types. Although very difficult, the analysis for N < 0 is a bit more
  // tedious due to extra subtractions in quotient() and remainder(). It
  // shows that the results are correct for N >= -2^(S - 3) + 365.

  if (N < limits<T>::N_min || limits<T>::N_max < N)
    std::cout << "WARNING: Rata die is out of bounds. (The code has "
      "undefined behaviour.)\n";

  // Year.
  T        const N_1 = 4 * N + 3;
  T        const Y   = quotient(N_1, 1461);
  uint32_t const N_Y = remainder(N_1, 1461) / 4;

  // Month and day.
  uint32_t const N_2 = 5 * N_Y + 461;
  uint32_t const M   = N_2 / 153;
  uint32_t const D   = N_2 % 153 / 5;

  // Map.
  uint32_t const J   = M >= 13;
  T        const Y_J = Y + J;
  uint32_t const M_J = M - 12 * J;
  uint32_t const D_J = D + 1;

  return { Y_J, M_J, D_J };
}

/**
 * @brief Calculates the rata die of a given proleptic Julian date.
 *
 * The epoch is 1 March 0000 of the proleptic Julian calendar.
 *
 * @tparam T        Year and rata die type.
 *
 * @param Y_J       The year.
 * @param M_J       The month.
 * @param D_J       The day.
 *
 * @return The proleptic Julian date.
 */
template <typename T>
T constexpr
to_rata_die(T Y_J, uint32_t M_J, uint32_t D_J) noexcept {

  date_t<T> const date = { Y_J, M_J, D_J };

  // Let S in {32, 64} be the size of T in bits. If N Y=0, then the
  // evaluation of y_star overflows if, and only if, 1461 * Y >= 2^(S - 1)
  // -- one more than the maximum value of T. This happens, if and only if,
  // Y >= 2^(S - 1) / 1461 + 1.
  //
  // If the month is January or February, then
  //   Y >= 2^(S - 1) / 1461 + 1 <=> Y_J >= 2^(S - 1) / 1461 + 2.
  //
  // Otherwise,
  //   Y >= 2^(S - 1) / 1461 + 1 <=> Y_J >= 2^(S - 1) / 1461 + 1.
  //
  // Hence, the code overflows from 1 March 2^(S - 1) / 1461 + 1.
  // Reciprocally, if the given date is before 1 March 2^(S - 1) / 1461 + 1,
  // then the calculation of y_star does not overflow. All other
  // calculations are safe since they work on numbers comfortably inside the
  // range of their types. Although very difficult, the analysis for Y < 0
  // is a bit more tedious due to an extra subtraction in quotient(). It
  // shows that the results are correct for dates down to
  // 1 March -2^(S - 1) + 3.

  date_t<T> constexpr date_min = { limits<T>::year_min, 3,  1 };
  date_t<T> constexpr date_max = { limits<T>::year_max, 2, 28 };

  if (date < date_min || date_max < date)
    std::cout << "WARNING: Date is out of bounds. (The code has undefined "
      "behaviour.)\n";

  // Map.
  uint32_t const J = M_J <= 2;
  T        const Y = Y_J - J;
  uint32_t const M = M_J + 12 * J;
  uint32_t const D = D_J - 1;


  // Rata die.
  T        const y_star = quotient(1461 * Y, 4);
  uint32_t const m_star = (153 * M - 457) / 5;
  T        const N      = y_star + m_star + D;

  return N;
}

} // namespace julian
} // namespace eaf

#endif // EAF_JULIAN_HPP
