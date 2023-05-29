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

#include "eaf/date.hpp"
#include "eaf/gregorian.hpp"
#include "eaf/julian.hpp"

#include <cstdint>

namespace eaf {
namespace info {

template <typename T>
struct julian_t : limits<T> {

  static date_t<T> constexpr
  to_date(T N) noexcept {
    return ::eaf::julian::to_date(N);
  }

  T constexpr
  static to_rata_die(date_t<T> date) noexcept {
    return ::eaf::julian::to_rata_die(date.year, date.month, date.day);
  }

};

template <typename T>
struct gregorian_t : limits<T> {

  static date_t<T> constexpr
  to_date(T N) noexcept {
    return ::eaf::gregorian::to_date(N);
  }

  T constexpr
  static to_rata_die(date_t<T> date) noexcept {
    return ::eaf::gregorian::to_rata_die(date.year, date.month, date.day);
  }

};

template <typename T, T epoch = 0, T s = 0>
struct gregorian_opt_t : limits_gregorian_opt<T, epoch, s> {

  static date_t<T> constexpr
  to_date(T N) noexcept {
    return ::eaf::gregorian::to_date_opt<T, epoch, s>(N);
  }

  T constexpr
  static to_rata_die(date_t<T> date) noexcept {
    return ::eaf::gregorian::to_rata_die_opt<T, epoch, s>(date.year,
      date.month, date.day);
  }

};

template <typename TAlgorithm>
void print() {

  using algorithm_t = TAlgorithm;

  std::cout << "  to_date\n";
  std::cout << "    rata_die_min          = " <<
    algorithm_t::rata_die_min << '\n';
  std::cout << "    rata_die_max          = " <<
    algorithm_t::rata_die_max << '\n';
  std::cout << "    to_date(rata_die_min) = " <<
    algorithm_t::to_date(algorithm_t::rata_die_min) << '\n';
  std::cout << "    to_date(rata_die_max) = " <<
    algorithm_t::to_date(algorithm_t::rata_die_max) << '\n';

  std::cout << "  to_rata_die\n";
  std::cout << "    date_min              = " <<
    algorithm_t::date_min << '\n';
  std::cout << "    date_max              = " <<
    algorithm_t::date_max << '\n';
  std::cout << "    to_rata_die(date_min) = " <<
    algorithm_t::to_rata_die(algorithm_t::date_min) << '\n';
  std::cout << "    to_rata_die(date_max) = " <<
    algorithm_t::to_rata_die(algorithm_t::date_max) << '\n';
}

} // namespace info
} // namespace eaf

int main() {

  using namespace ::eaf::info;

  std::cout << "Julian 32-bits:\n";
  print<julian_t<int32_t>>();

  std::cout << '\n';

  std::cout << "Julian 64-bits:\n";
  print<julian_t<int64_t>>();

  std::cout << '\n';

  std::cout << "Gregorian 32-bits:\n";
  print<gregorian_t<int32_t>>();

  std::cout << '\n';

  std::cout << "Gregorian 64-bits:\n";
  print<gregorian_t<int64_t>>();

  std::cout << '\n';

  std::cout << "Gregorian optimised 32-bits:\n";
  print<gregorian_opt_t<int32_t>>();

  std::cout << '\n';

  std::cout << "Gregorian optimised 64-bits:\n";
  print<gregorian_opt_t<int64_t>>();

  std::cout << '\n';

  std::cout << "Gregorian (Unix) optimised 32-bits:\n";
  print<gregorian_opt_t<int32_t, 719468, 82>>();

  std::cout << '\n';

  std::cout << "Gregorian (Unix) optimised 64-bits:\n";
  print<gregorian_opt_t<int64_t, 719468, 82>>();

  return 0;
}
