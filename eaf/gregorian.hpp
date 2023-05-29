// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file gregorian.hpp
 *
 * @brief Algorithms on the Gregorian calendar.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_GREGORIAN_HPP
#define EAF_GREGORIAN_HPP

#include "eaf/common.hpp"
#include "eaf/limits.hpp"

#include <cstdint>
#include <limits>
#include <iostream>

namespace eaf {
namespace gregorian {

/**
 * @brief Finds the proleptic Gregorian date from its rata die.
 *
 * The epoch is 1 March 0000 of the proleptic Gregorian calendar.
 *
 * @tparam T        Year and rata die type.
 *
 * @param N         The rata die.
 *
 * @return The proleptic Gregorian date.
 */
template <typename T>
date_t<T> constexpr
to_date(T N) noexcept {

  if (N < limits<T>::rata_die_min || limits<T>::rata_die_max < N)
    std::cout << "WARNING: Rata die is out of bounds. (The code has "
      "undefined behaviour.)\n";

  // Century.
  T        const N_1 = 4 * N + 3;
  T        const C   = quotient(N_1, 146097);
  uint32_t const N_C = remainder(N_1, 146097) / 4;

  // Year.
  uint32_t const N_2 = 4 * N_C + 3;
  uint32_t const Z   = N_2 / 1461;
  uint32_t const N_Y = N_2 % 1461 / 4;
  T        const Y   = 100 * C + Z;

  // Month and day.
  uint32_t const N_3 = 5 * N_Y + 461;
  uint32_t const M   = N_3 / 153;
  uint32_t const D   = N_3 % 153 / 5;

  // Map.
  uint32_t const J   = M >= 13;
  T        const Y_G = Y + J;
  uint32_t const M_G = M - 12 * J;
  uint32_t const D_G = D + 1;

  return { Y_G, M_G, D_G };
}

/**
 * @brief Calculates the rata die of a given proleptic Gregorian date.
 *
 * The epoch is 1 March 0000 of the proleptic Gregorian calendar.
 *
 * @tparam T        Year and rata die type.
 *
 * @param Y_G       The year.
 * @param M_G       The month.
 * @param D_G       The day.
 *
 * @return The rata die.
 */
template <typename T>
T constexpr
to_rata_die(T Y_G, uint32_t M_G, uint32_t D_G) noexcept {

  date_t<T> const date = { Y_G, M_G, D_G };
  if (date < limits<T>::date_min || limits<T>::date_max < date)
    std::cout << "WARNING: Date is out of bounds. (The code has undefined "
      "behaviour.)\n";

  // Map.
  uint32_t const J = M_G <= 2;
  T        const Y = Y_G - J;
  uint32_t const M = M_G + 12 * J;
  uint32_t const D = D_G - 1;
  T        const C = quotient(Y, 100);

  // Rata die.
  T        const y_star = quotient(1461 * Y, 4) - C + quotient(C, 4);
  uint32_t const m_star = (153 * M - 457) / 5;
  T        const N      = y_star + m_star + D;

  return N;
}

/**
 * @brief Finds the proleptic Gregorian date from its rata die w.r.t. a
 * customised epoch. (Section 11 of the paper.)
 *
 * The epoch is 1 March 0000 of the proleptic Gregorian calendar.
 *
 * @tparam T        Year and rata die type.
 * @tparam epoch    The epoch shift (e.g., 719468 for Unix epoch).
 * @tparam s        Cycles shift.
 *
 * @param N         The rata die.
 *
 * @return The proleptic Gregorian date.
 */
template <typename T, T epoch = 0, T s = 0>
date_t<T> constexpr
to_date_opt(T N_U) noexcept {

  if (N_U < limits_gregorian_opt<T, epoch, s>::rata_die_min ||
      limits_gregorian_opt<T, epoch, s>::rata_die_max < N_U)
    std::cout << "WARNING: Rata die is out of bounds. (The code has "
      "undefined behaviour.)\n";

  using uint_t = typename std::make_unsigned<T>::type;

  // Shift and correction constants.
  T constexpr K = epoch + 146097 * s;
  T constexpr L = 400 * s;

  // Rata die shift.
  uint_t   const N   = uint_t(N_U) + uint_t(K);

  // Century.
  uint_t   const N_1 = 4 * N + 3;
  uint_t   const C   = N_1 / 146097;
  uint32_t const N_C = N_1 % 146097 / 4;

  // Year.
  uint32_t const N_2 = 4 * N_C + 3;
  uint64_t const P_2 = uint64_t(2939745) * N_2;
  uint32_t const Z   = uint32_t(P_2 / 4294967296);
  uint32_t const N_Y = uint32_t(P_2 % 4294967296) / 2939745 / 4;
  uint_t   const Y   = 100 * C + Z;

  // Month and day.
  uint32_t const N_3 = 2141 * N_Y + 197913;
  uint32_t const M   = N_3 / 65536;
  uint32_t const D   = N_3 % 65536 / 2141;

  // Map.
  uint32_t const J   = N_Y >= 306;
  T        const Y_G = (Y - L) + J;
  uint32_t const M_G = J ? M - 12 : M;
  uint32_t const D_G = D + 1;

  return { Y_G, M_G, D_G };
}

/**
 * @brief Calculates the rata die w.r.t. a customised epoch of a given
 * proleptic Gregorian date. (Section 11 of the paper.)
 *
 * The epoch is 1 March 0000 of the proleptic Gregorian calendar.
 *
 * @tparam T        Year and rata die type.
 * @tparam epoch    The epoch shift (e.g., 719468 for Unix epoch).
 * @tparam s        Cycles shift.
 *
 * @param Y_G       The year.
 * @param M_G       The month.
 * @param D_G       The day.
 *
 * @return The rata die.
 */
template <typename T, T epoch = 0, T s = 0>
T constexpr
to_rata_die_opt(T Y_G, uint32_t M_G, uint32_t D_G) noexcept {

  date_t<T> const date = { Y_G, M_G, D_G };
  if (date < limits_gregorian_opt<T, epoch, s>::date_min ||
      limits_gregorian_opt<T, epoch, s>::date_max < date)
    std::cout << "WARNING: Date is out of bounds. (The code has undefined "
      "behaviour.)\n";

  using uint_t = typename std::make_unsigned<T>::type;

  // Shift and correction constants.
  T constexpr K = epoch + 146097 * s;
  T constexpr L = 400 * s;

  // Map.
  uint32_t const J = M_G <= 2;
  uint_t   const Y = uint_t(Y_G) + uint_t(L) - J;
  uint32_t const M = J ? M_G + 12 : M_G;
  uint32_t const D = D_G - 1;
  uint_t   const C = Y / 100;

  // Rata die.
  uint_t   const y_star = 1461 * Y / 4 - C + C / 4;
  uint32_t const m_star = (153 * M - 457) / 5;
  uint_t   const N      = y_star + m_star + D;

  // Rata die shift.
  uint_t   const N_U    = N - K;

  return N_U;
}

} // namespace gregorian
} // namespace eaf

#endif // EAF_GREGORIAN_HPP
