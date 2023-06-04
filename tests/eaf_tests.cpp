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

  static date32_t constexpr epoch = { 0, 3, 1 };

  static date32_t constexpr
  to_date(int32_t N) noexcept {
    return ::eaf::julian::to_date(N);
  }

  int32_t constexpr
  static to_rata_die(int32_t Y_J, uint32_t M_J, uint32_t D_J) noexcept {
    return ::eaf::julian::to_rata_die(Y_J, M_J, D_J);
  }

};

date32_t constexpr julian::epoch;

struct gregorian : gregorian_helper_t, limits<int32_t> {

  static date32_t constexpr epoch = { 0, 3, 1 };

  static date32_t constexpr
  to_date(int32_t N) noexcept {
    return ::eaf::gregorian::to_date(N);
  }

  int32_t constexpr
  static to_rata_die(int32_t Y_J, uint32_t M_J, uint32_t D_J) noexcept {
    return ::eaf::gregorian::to_rata_die(Y_J, M_J, D_J);
  }

};

date32_t constexpr gregorian::epoch;

struct gregorian_opt : gregorian_helper_t, limits_gregorian_opt<int32_t> {

  static date32_t constexpr epoch = { 0, 3, 1 };

  static date32_t constexpr
  to_date(int32_t N) noexcept {
    return ::eaf::gregorian::to_date_opt(N);
  }

  int32_t constexpr
  static to_rata_die(int32_t Y_J, uint32_t M_J, uint32_t D_J) noexcept {
    return ::eaf::gregorian::to_rata_die_opt(Y_J, M_J, D_J);
  }

};

date32_t constexpr gregorian_opt::epoch;

struct gregorian_unix :
  gregorian_helper_t,
  limits_gregorian_opt<int32_t, 719468, 82> {

  static date32_t constexpr epoch = { 1970, 1, 1 };

  static date32_t constexpr
  to_date(int32_t N) noexcept {
    return ::eaf::gregorian::to_date_opt<int32_t, 719468, 82>(N);
  }

  int32_t constexpr
  static to_rata_die(int32_t Y_J, uint32_t M_J, uint32_t D_J) noexcept {
    return ::eaf::gregorian::to_rata_die_opt<int32_t, 719468, 82>(Y_J, M_J,
      D_J);
  }

};

date32_t constexpr gregorian_unix::epoch;

template <typename A>
struct eaf_tests : public ::testing::Test {
}; // struct eaf_tests

using calendars = ::testing::Types<
  julian,
  gregorian,
  gregorian_opt,
  gregorian_unix
>;

// The extra comma below is to silent a warning.
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(eaf_tests, calendars, );

/**
 * Tests that epoch is mapped to 0.
 */
TYPED_TEST(eaf_tests, epoch) {

  using calendar_t = TypeParam;

  EXPECT_EQ(calendar_t::to_date(0), calendar_t::epoch);
  EXPECT_EQ(to_rata_die<calendar_t>(calendar_t::epoch), 0);
}

/**
 * Tests to_date going forward from 0 to rata_die_max.
 */
TYPED_TEST(eaf_tests, to_date_forward) {

  using calendar_t = TypeParam;

  date32_t date = calendar_t::epoch;
  for (int32_t n = 0; n < calendar_t::rata_die_max; ) {
    date32_t const tomorrow = calendar_t::to_date(++n);
    ASSERT_EQ(tomorrow, calendar_t::advance(date)) <<
      "Failed for rata_die = " << n;
  }
}

/**
 * Tests to_date going backward from 0 t rata_die_min.
 */
TYPED_TEST(eaf_tests, to_date_backward) {

  using calendar_t = TypeParam;

  date32_t date = calendar_t::epoch;
  for (int32_t n = 0; calendar_t::rata_die_min < n; ) {
    date32_t const yesterday = calendar_t::to_date(--n);
    ASSERT_EQ(yesterday, calendar_t::regress(date)) <<
      "Failed for rata_die = " << n;
  }
}

/**
 * Tests to_rata_die going forward from epoch to date_max.
 */
TYPED_TEST(eaf_tests, to_rata_die_forward) {

  using calendar_t = TypeParam;

  int32_t n = 0;
  for (date32_t date = calendar_t::epoch; date < calendar_t::date_max; ) {
    int32_t const tomorrow =
      to_rata_die<calendar_t>(calendar_t::advance(date));
    ASSERT_EQ(tomorrow, ++n) << "Failed for date = " << date;
  }
}

/**
 * Tests to_rata_die going backward from epoch to date_min.
 */
TYPED_TEST(eaf_tests, to_rata_die_backward) {

  using calendar_t = TypeParam;

  int32_t n = 0;
  for (date32_t date = calendar_t::epoch; calendar_t::date_min < date; ) {
    int32_t const yesterday =
      to_rata_die<calendar_t>(calendar_t::regress(date));
    ASSERT_EQ(yesterday, --n) << "Failed for date = " << date;
  }
}

} // namespace tests
} // namespace eaf
