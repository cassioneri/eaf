// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2022 Cassio Neri <cassio.neri@gmail.com>
// SPDX-FileCopyrightText: 2022 Lorenz Schneider <schneider@em-lyon.com>

/**
 * @file algorithm_tests.cpp
 *
 * @brief Command line program that tests implementations of ours and
 *   others' calendar algorithms.
 *
 * This code is a supplementary material to:
 *
 *     Neri C, and Schneider L, "Euclidean Affine Functions and their
 *     Application to Calendar Algorithms" (2022).
 */

#include "tests/tests.hpp"

#include "algorithms/baum.hpp"
#include "algorithms/boost.hpp"
#include "algorithms/dotnet.hpp"
#include "algorithms/fliegel_flandern.hpp"
#include "algorithms/glibc.hpp"
#include "algorithms/hatcher.hpp"
#include "algorithms/libcxx.hpp"
#include "algorithms/neri_schneider.hpp"
#include "algorithms/openjdk.hpp"
#include "algorithms/reingold_dershowitz.hpp"
#include "eaf/date.hpp"

#include <gtest/gtest.h>

#include <cstdint>

namespace eaf {
namespace tests {

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
// Algorithm tests
//--------------------------------------------------------------------------

template <typename A>
struct algorithm_tests : public ::testing::Test {
}; // struct algorithm_tests

using implementations = ::testing::Types<
  baum,
  boost,
  dotnet,
  fliegel_flandern,
  glibc,
  hatcher,
  libcxx,
  neri_schneider,
  openjdk,
  reingold_dershowitz
>;

// The extra comma below is to silent a warning.
// https://github.com/google/googletest/issues/2271#issuecomment-665742471
TYPED_TEST_SUITE(algorithm_tests, implementations, );

static date32_t constexpr unix_epoch = { 1970, 1, 1 };

/**
 * Tests whether epoch is mapped to 0.
 */
TYPED_TEST(algorithm_tests, epoch) {

  using algoritm_t = TypeParam;

  EXPECT_EQ(algoritm_t::to_date(0), unix_epoch);
  EXPECT_EQ(algoritm_t::to_rata_die(1970, 1, 1), 0);
}

/**
 * Tests whether to_date produces correct results going forward from 0 to
 * rata_die_max.
 */
TYPED_TEST(algorithm_tests, to_date_forward) {

  using algoritm_t     = TypeParam;
  int32_t rata_die_max = limits<algoritm_t>::rata_die_max;

  date32_t date = unix_epoch;
  for (int32_t n = 0; n < rata_die_max; ) {
    date32_t const tomorrow = algoritm_t::to_date(++n);
    ASSERT_EQ(tomorrow, gregorian_helper_t::advance(date)) <<
      "Failed for rata_die = " << n;
  }
}

/**
 * Tests whether to_date produces correct results going backward from 0 t
 * rata_die_min.
 */
TYPED_TEST(algorithm_tests, to_date_backward) {

  using algoritm_t     = TypeParam;
  int32_t rata_die_min = limits<algoritm_t>::rata_die_min;

  date32_t date = unix_epoch;
  for (int32_t n = 0; rata_die_min < n; ) {
    date32_t const yesterday = algoritm_t::to_date(--n);
    ASSERT_EQ(yesterday, gregorian_helper_t::regress(date)) <<
      "Failed for rata_die = " << n;
  }
}

/**
 * Tests whether to_rata_die produce correct results going foward from epoch
 * to date_max.
 */
TYPED_TEST(algorithm_tests, to_rata_die_forward) {

  using algoritm_t  = TypeParam;
  date32_t date_max = limits<algoritm_t>::date_max;

  int32_t n = 0;
  for (date32_t date = unix_epoch; date < date_max; ) {
    int32_t const tomorrow =
      to_rata_die<algoritm_t>(gregorian_helper_t::advance(date));
    ASSERT_EQ(tomorrow, ++n) << "Failed for date = " << date;
  }
}

/**
 * Tests whether to_rata_die produce correct results backward from epoch to
 * date_min.
 */
TYPED_TEST(algorithm_tests, to_rata_die_backward) {

  using algoritm_t  = TypeParam;
  date32_t date_min = limits<algoritm_t>::date_min;

  int32_t n = 0;
  for (date32_t date = unix_epoch; date_min < date; ) {
    int32_t const yesterday =
      to_rata_die<algoritm_t>(gregorian_helper_t::regress(date));
    ASSERT_EQ(yesterday, --n) << "Failed for date = " << date;
  }
}

} // namespace tests
} // namespace eaf
