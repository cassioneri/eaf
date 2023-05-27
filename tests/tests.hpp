// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file common.hpp
 *
 * @brief Common utilities.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#ifndef EAF_TESTS_TESTS_HPP
#define EAF_TESTS_TESTS_HPP

#include "eaf/date.hpp"

#include <cstdint>

namespace eaf {

//--------------------------------------------------------------------------
// Helpers
//--------------------------------------------------------------------------

/**
 * @brief Checks whether a given year is leap or not.
 *
 * @param y          The given year.
 */
bool is_leap_year(int32_t y) {
  // Originally, the ternary expression was similar to
  //   y % 100 == 0 ? y % 16 == 0 : y % 4 == 0;
  // and Ulrich Drepper suggested the following twist.
  return (uint32_t(y) & (y % 100 == 0 ? 15 : 3)) == 0;
}

/**
 * @brief Returns the last day of the month for a given year and month.
 *
 * @param y          The given year.
 * @param m          The given month.
 */
uint32_t last_day_of_month(int32_t y, uint32_t m) {
  // Originally the 2nd operand of the 1st ternary operator was
  //   (month ^ (month >> 3)) & 1 | 30
  // and Dr. Matthias Kretz realised '& 1' was unnecessary.
  return m != 2 ? ((m ^ (m >> 3))) | 30 : is_leap_year(y) ? 29 : 28;
}

/**
 * Advances a date by one day.
 *
 * @param date       Date to be advanced.
 *
 * @pre              date < max<date_t>.
 */
date32_t advance(date32_t& date) {
  if (date.day != last_day_of_month(date.year, date.month))
    ++date.day;
  else {
    date.day = 1;
    if (date.month != 12)
      ++date.month;
    else {
      date.month = 1;
      ++date.year;
    }
  }
  return date;
}

/**
 * Returns the next date.
 *
 * @param date       Date to be advanced.
 *
 * @pre              date < max<date_t>.
 */
date32_t next(date32_t date) {
  return advance(date);
}

/**
 * Regresses a date by one day.
 *
 * @param date       Date to be regressed.
 *
 * @pre              date > min<date_t>.
 */
date32_t regress(date32_t& date) {
  if (date.day != 1)
    --date.day;
  else {
    if (date.month != 1)
      --date.month;
    else {
      date.month = 12;
      --date.year;
    }
    date.day = last_day_of_month(date.year, date.month);
  }
  return date;
}

/**
 * Returns the previous date.
 *
 * @param date       Date to be regressed.
 *
 * @pre              date > min<date_t>.
 */
date32_t previous(date32_t date) {
  return regress(date);
}

  // Used in tests.
template <typename T>
int32_t to_rata_die(date32_t date) {
  return T::to_rata_die(date.year, date.month, date.day);
}

} // namespace eaf

#endif // EAF_TESTS_TESTS_HPP
