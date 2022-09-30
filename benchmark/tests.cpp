// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file tests.cpp
 *
 * @brief Command line program that tests many calendar algorithms.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "date.hpp"
#include "baum.hpp"
#include "boost.hpp"
#include "dotnet.hpp"
#include "fliegel_flandern.hpp"
#include "glibc.hpp"
#include "hatcher.hpp"
#include "libcxx.hpp"
#include "neri_schneider.hpp"
#include "openjdk.hpp"
#include "reingold_dershowitz.hpp"

#include <gtest/gtest.h>

#include <cstdint>

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

//--------------------------------------------------------------------------
// Limits
//--------------------------------------------------------------------------

// All algorithms but ours are tested for a range spanning 800 years
// centered in 1 January 1970.
template <typename T>
struct limits {
  date32_t static constexpr date_min     = {  1970 - 400,  1,  1 };
  date32_t static constexpr date_max     = {  1970 + 400,  1,  1 };
  int32_t  static constexpr rata_die_min = -146097;
  int32_t  static constexpr rata_die_max =  146097;
};

// Our algorithms are tested in their whole range of validity. (This is much
// larger than the 800 years we use for others.)
template <>
struct limits<neri_schneider> {
  date32_t static constexpr date_min     = neri_schneider::date_min;
  date32_t static constexpr date_max     = neri_schneider::date_min;
  int32_t  static constexpr rata_die_min = neri_schneider::rata_die_min;
  int32_t  static constexpr rata_die_max = neri_schneider::rata_die_max;
};

//--------------------------------------------------------------------------
// Calendar tests
//--------------------------------------------------------------------------

template <typename A>
struct calendar_tests : public ::testing::Test {
}; // struct calendar_tests

using implementations = ::testing::Types<
  baum,
  boost,
  dotnet,
  fliegel_flandern,
  glibc,
  hatcher,
  neri_schneider,
  openjdk,
  reingold_dershowitz
>;

// The extra comma below is to silent a warning.
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(calendar_tests, implementations, );

static date32_t constexpr unix_epoch = { 1970, 1, 1 };

/**
 * Tests whether epoch is mapped to 0.
 */
TYPED_TEST(calendar_tests, epoch) {

  using algoritm_t = TypeParam;

  EXPECT_EQ(algoritm_t::to_date(0), unix_epoch);
  EXPECT_EQ(algoritm_t::to_rata_die(1970, 1, 1), 0);
}

/**
 * Tests whether to_date produces correct results going forward from 0 to
 * rata_die_max.
 */
TYPED_TEST(calendar_tests, to_date_forward) {

  using algoritm_t     = TypeParam;
  int32_t rata_die_max = limits<algoritm_t>::rata_die_max;

  date32_t date = unix_epoch;
  for (int32_t n = 0; n < rata_die_max; ) {
    date32_t const tomorrow = algoritm_t::to_date(++n);
    ASSERT_EQ(tomorrow, advance(date)) << "Failed for rata_die = " << n;
  }
}

/**
 * Tests whether to_date produces correct results going backward from 0 t
 * rata_die_min.
 */
TYPED_TEST(calendar_tests, to_date_backward) {

  using algoritm_t     = TypeParam;
  int32_t rata_die_min = limits<algoritm_t>::rata_die_min;

  date32_t date = unix_epoch;
  for (int32_t n = 0; rata_die_min < n; ) {
    date32_t const yesterday = algoritm_t::to_date(--n);
    ASSERT_EQ(yesterday, regress(date)) << "Failed for rata_die = " << n;
  }
}

/**
 * Tests whether to_rata_die produce correct results going foward from epoch to date_max.
 */
TYPED_TEST(calendar_tests, to_rata_die_forward) {

  using algoritm_t  = TypeParam;
  date32_t date_max = limits<algoritm_t>::date_max;

  int32_t n = 0;
  for (date32_t date = unix_epoch; date < date_max; ) {
    int32_t const tomorrow = to_rata_die<algoritm_t>(advance(date));
    ASSERT_EQ(tomorrow, ++n) << "Failed for date = " << date;
  }
}

/**
 * Tests whether to_rata_die produce correct results backward from epoch to date_min.
 */
TYPED_TEST(calendar_tests, to_rata_die_backward) {

  using algoritm_t  = TypeParam;
  date32_t date_min = limits<algoritm_t>::date_min;

  int32_t n = 0;
  for (date32_t date = unix_epoch; date_min < date; ) {
    int32_t const yesterday = to_rata_die<algoritm_t>(regress(date));
    ASSERT_EQ(yesterday, --n) << "Failed for date = " << date;
  }
}
