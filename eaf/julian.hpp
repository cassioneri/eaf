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

#ifndef EAF_EAF_JULIAN_HPP
#define EAF_EAF_JULIAN_HPP

#include "eaf/common.hpp"
#include "eaf/limits.hpp"

#include <cstdint>
#include <limits>
#include <iostream>

namespace eaf {
namespace julian {

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

  if (N < limits<T>::rata_die_min || limits<T>::rata_die_max < N)
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
  if (date < limits<T>::date_min || limits<T>::date_max < date)
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

#endif // EAF_EAF_JULIAN_HPP
