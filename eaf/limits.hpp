// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file Limits.hpp
 *
 * @brief Limits for EAF based calendar algorithms.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_EAF_LIMITS_HPP
#define EAF_EAF_LIMITS_HPP

namespace eaf {

/**
 * @brief Limits for the (non optimised) Julian and Gregorian algorithms.
 *
 * Contains static \c constexpr data members \c rata_die_min,
 * \c rata_die_min, \c date_min and \c date_max, so that:
 *
 * \li <tt>to_date(N)</tt> is correct for
 *     <tt>rata_die_min <= N <= rata_die_max</tt>;
 *
 * \li <tt>to_rata_die(Y_{G/J}, M_{G/J}, D_{G/J})</tt> is correct for
 *     <tt>date_min <= date <= date_max</tt>, where
 *     <tt>date = date_t<T>{ Y_{G/J}, M_{G/J}, D_{G/J} }</tt> is either in
 *     the Julian or Gregorian calendar.
 *
 * @tparam T        Year and rata die type.
 */
template <typename T>
class limits {

  static T constexpr max = std::numeric_limits<T>::max();
  static T constexpr min = std::numeric_limits<T>::min();

public:

  // Assume N >= 0. Then, N_1 = 4 * N + 3 does not overflow if, and only if:
  //     4 * N + 3 <= max <=> 4 * N <= max - 3 <=>
  //     N <= (max - 3) / 4 (truncated division.)
  static T constexpr rata_die_max = (max - 3) / 4;

  // Assume N < 0. Then, N_1 = 4 * N + 3 does not overflow if, and only if
  // 4 * N does not overflow, that is:
  //     4 * N >= min <=> N >= min / 4 (truncated division.)
  static T constexpr rata_die_min = min / 4;

  // Assume Y >= 0. Then, 1461 * Y does not overflow if, and only if,
  //     1461 * Y <= max <=> Y <= max / 1461 (truncated division.)
  // If the month is Jan or Feb, then
  //     Y <= max / 1461 <=> Y_{G/J} <= max / 1461 + 1.
  // Otherwise,
  //     Y <= max / 1461 <=> Y_{G/J} <= max / 1461.
  // Hence, there's no overflow up to 28 Feb of year max / 1461 + 1.
  // (If year max / 1461 + 1 is a leap year, then there's no overflow on 29
  // Feb of this year either, so the bound below might not be sharp.)
  static date_t<T> constexpr date_max = { max / 1461 + 1, 2, 28 };

  // Assume Y < 0. Then, 1461 * Y overflows if, and only if,
  //     1461 * Y < min <=> Y < min / 1461.
  // If the month is Jan or Feb, then
  //     Y < min / 1461 <=> Y_{G/J} < min / 1461 + 1.
  // Otherwise,
  //     Y > min / 1461 <=> Y_{G/J} < min / 1461.
  // Hence, there's overflow on the last day of Feb of Y_{G/J} = min / 1461.
  static date_t<T> constexpr date_min = { min / 1461, 3, 1 };
};

} // namespace eaf

#endif // EAF_EAF_LIMITS_HPP