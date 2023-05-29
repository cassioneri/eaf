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
#include "eaf/gregorian.hpp"
#include "eaf/julian.hpp"

#include <gtest/gtest.h>

#include <cstdint>

namespace eaf {
namespace tests {

struct julian : julian_helper_t, limits<int32_t> {

  static date32_t constexpr
  to_date(int32_t N) noexcept {
    return ::eaf::julian::to_date(N);
  }

  int32_t constexpr
  static to_rata_die(int32_t Y_J, uint32_t M_J, uint32_t D_J) noexcept {
    return ::eaf::julian::to_rata_die(Y_J, M_J, D_J);
  }

};

struct gregorian : gregorian_helper_t, limits<int32_t> {

  static date32_t constexpr
  to_date(int32_t N) noexcept {
    return ::eaf::gregorian::to_date(N);
  }

  int32_t constexpr
  static to_rata_die(int32_t Y_J, uint32_t M_J, uint32_t D_J) noexcept {
    return ::eaf::gregorian::to_rata_die(Y_J, M_J, D_J);
  }

};

template <typename A>
struct eaf_tests : public ::testing::Test {
}; // struct eaf_tests

using calendars = ::testing::Types<
  julian,
  gregorian
>;

// The extra comma below is to silent a warning.
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(eaf_tests, calendars, );

static date32_t constexpr epoch = { 0, 3, 1 };

/**
 * Tests whether epoch is mapped to 0.
 */
TYPED_TEST(eaf_tests, epoch) {

  using calendar_t = TypeParam;

  EXPECT_EQ(calendar_t::to_date(0), epoch);
  EXPECT_EQ(calendar_t::to_rata_die(0, 3, 1), 0);
}

/**
 * Tests whether to_date produces correct results going forward from 0 to
 * rata_die_max.
 */
TYPED_TEST(eaf_tests, to_date_forward) {

  using calendar_t = TypeParam;

  date32_t date = epoch;
  for (int32_t n = 0; n < calendar_t::rata_die_max; ) {
    date32_t const tomorrow = calendar_t::to_date(++n);
    ASSERT_EQ(tomorrow, calendar_t::advance(date)) <<
      "Failed for rata_die = " << n;
  }
}

/**
 * Tests whether to_date produces correct results going backward from 0 t
 * rata_die_min.
 */
TYPED_TEST(eaf_tests, to_date_backward) {

  using calendar_t = TypeParam;

  date32_t date = epoch;
  for (int32_t n = 0; calendar_t::rata_die_min < n; ) {
    date32_t const yesterday = calendar_t::to_date(--n);
    ASSERT_EQ(yesterday, calendar_t::regress(date)) <<
      "Failed for rata_die = " << n;
  }
}

/**
 * Tests whether to_rata_die produce correct results going foward from epoch to date_max.
 */
TYPED_TEST(eaf_tests, to_rata_die_forward) {

  using calendar_t = TypeParam;

  int32_t n = 0;
  for (date32_t date = epoch; date < calendar_t::date_max; ) {
    int32_t const tomorrow =
      to_rata_die<calendar_t>(calendar_t::advance(date));
    ASSERT_EQ(tomorrow, ++n) << "Failed for date = " << date;
  }
}

/**
 * Tests whether to_rata_die produce correct results backward from epoch to date_min.
 */
TYPED_TEST(eaf_tests, to_rata_die_backward) {

  using calendar_t  = TypeParam;

  int32_t n = 0;
  for (date32_t date = epoch; calendar_t::date_min < date; ) {
    int32_t const yesterday =
      to_rata_die<calendar_t>(calendar_t::regress(date));
    ASSERT_EQ(yesterday, --n) << "Failed for date = " << date;
  }
}

} // namespace tests
} // namespace eaf
