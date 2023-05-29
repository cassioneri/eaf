// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file eaf_tests.cpp
 *
 * @brief Command line program that tests many calendar algorithms.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "tests/tests.hpp"

#include "eaf/date.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#if 0
namespace eaf {
namespace tests {

struct julian {
  using helper_t = helper_t<julian_leap_t>;
};

struct gregorian {
  using helper_t = helper_t<gregorian_leap_t>;
};

template <typename A>
struct calendar_tests : public ::testing::Test {
}; // struct calendat_tests

using calendars = ::testing::Types<
  julian,
  gregorian
>;

// The extra comma below is to silent a warning.
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(calendar_tests, calendars, );

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

} // namespace tests
} // namespace eaf
#endif
